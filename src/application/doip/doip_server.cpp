#include <algorithm>
#include <stdexcept>
#include "./doip_server.h"

namespace application
{
    namespace doip
    {
        constexpr size_t DoipServer::cDoipPacketSize;

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
                            mVehicleIdRequestHandler(config.protocolVersion, std::move(vin), logicalAddress, eid, gid),
                            mDiagMessageHandler(curl, resourcesUrl, config.protocolVersion),
                            mListener(ipAddress, port),
                            mController(std::move(config))
        {
            bool _successful{mListener.TrySetup()};

            if (_successful)
            {
                auto _onAccept{std::bind(&DoipServer::onAccept, this)};
                _successful = mPoller->TryAddListener(&mListener, _onAccept);

                if (_successful)
                {
                    const DoipLib::PayloadType cVehicleIdRequestPayload{
                        DoipLib::PayloadType::VehicleIdRequest};
                    mController.Register(
                        cVehicleIdRequestPayload, &mVehicleIdRequestHandler);

                    const DoipLib::PayloadType cDiagMessagePayload{
                        DoipLib::PayloadType::DiagMessage};
                    mController.Register(
                        cDiagMessagePayload, &mDiagMessageHandler);
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