#include <algorithm>
#include <stdexcept>
#include "./doip_server.h"

namespace application
{
    namespace doip
    {
        constexpr size_t DoipServer::cMacAddressSize;
        constexpr size_t DoipServer::cDoipPacketSize;
        const uint8_t DoipServer::cFurtherAction;

        DoipServer::DoipServer(
            AsyncBsdSocketLib::Poller *poller,
            helper::CurlWrapper *curl,
            std::string resourcesUrl,
            std::string ipAddress,
            uint16_t port,
            DoipLib::ControllerConfig &&config,
            std::string &&vin,
            uint16_t logicalAddress,
            uint64_t eid,
            uint64_t gid) : mPoller{poller},
                            mMessageHandler(curl, resourcesUrl, config.protocolVersion),
                            mListener(ipAddress, port),
                            mAnnouncement(config.protocolVersion, std::move(vin), logicalAddress, convertToMacAddress(eid), convertToMacAddress(gid), cFurtherAction),
                            mController(std::move(config))
        {
            bool _successful{mListener.TrySetup()};

            if (_successful)
            {
                auto _onAccept{std::bind(&DoipServer::onAccept, this)};
                _successful = mPoller->TryAddListener(&mListener, _onAccept);

                if (_successful)
                {
                    const DoipLib::PayloadType cPayloadType{
                        DoipLib::PayloadType::DiagMessage};

                    mController.Register(cPayloadType, &mMessageHandler);
                }
                else
                {
                    throw std::runtime_error(
                        "Adding the DoIP server to the poller failed.");
                }
            }
            else
            {
                throw std::runtime_error(
                    "DoIP server failed to listen on the given IP and port.");
            }
        }

        std::array<uint8_t, DoipServer::cMacAddressSize> DoipServer::convertToMacAddress(
            uint64_t id)
        {
            uint64_t _mask{0xff};
            std::array<uint8_t, cMacAddressSize> _result;

            for (size_t i = 0; i < cMacAddressSize; ++i)
            {
                const uint64_t cOctet{id & _mask};
                _result[i] = static_cast<uint8_t>(cOctet >> (i * 8));
                _mask <<= i * 8;
            }

            return _result;
        }

        void DoipServer::onAccept()
        {
            bool _successful{mListener.TryAccept()};
            if (!_successful)
            {
                return;
            }

            _successful = mListener.TryMakeConnectionNonblock();
            if (!_successful)
            {
                return;
            }

            auto _onReceive{std::bind(&DoipServer::onReceive, this)};
            _successful = mPoller->TryAddReceiver(&mListener, _onReceive);
            if (!_successful)
            {
                return;
            }

            auto _onSend{std::bind(&DoipServer::onSend, this)};
            mPoller->TryAddSender(&mListener, _onSend);
        }

        void DoipServer::onReceive()
        {
            std::array<uint8_t, cDoipPacketSize> _receiveBuffer;
            std::vector<uint8_t> _serializedResponse;

            mListener.Receive(_receiveBuffer);
            const std::vector<uint8_t> cSerializedRequest(
                _receiveBuffer.cbegin(), _receiveBuffer.cend());

            const bool cIsHandled{
                mController.TryHandle(cSerializedRequest, _serializedResponse)};
            if (cIsHandled)
            {
                mSendQueue.TryEnqueue(std::move(_serializedResponse));
            }
        }

        void DoipServer::onSend()
        {
            while (!mSendQueue.Empty())
            {
                std::vector<uint8_t> _sendData;
                const bool cDequeued{mSendQueue.TryDequeue(_sendData)};

                if (cDequeued && _sendData.size() <= cDoipPacketSize)
                {
                    std::array<uint8_t, cDoipPacketSize> _sendBuffer;
                    std::copy_n(
                        std::make_move_iterator(_sendData.begin()),
                        _sendData.size(),
                        _sendBuffer.begin());

                    mListener.Send(_sendBuffer);
                }

                std::this_thread::yield();
            }
        }

        DoipServer::~DoipServer()
        {
            AsyncBsdSocketLib::TcpListener *const cListenerPtr{&mListener};

            mPoller->TryRemoveSender(cListenerPtr);
            mPoller->TryRemoveReceiver(cListenerPtr);
            mPoller->TryRemoveListener(cListenerPtr);
        }
    }
}