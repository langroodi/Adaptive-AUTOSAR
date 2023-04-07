#include <algorithm>
#include <thread>
#include <doiplib/diag_message.h>
#include <doiplib/diag_message_ack.h>
#include <doiplib/vehicle_id_request.h>
#include <doiplib/vehicle_id_response.h>
#include "./doip_client.h"

namespace application
{
    namespace doip
    {
        constexpr size_t DoipClient::cDoipPacketSize;

        DoipClient::DoipClient(
            AsyncBsdSocketLib::Poller *poller,
            std::string ipAddress,
            uint16_t port,
            uint8_t protocolVersion,
            std::function<void(std::vector<uint8_t> &&)> &&callback) : mPoller{poller},
                                                                       mClient(ipAddress, port),
                                                                       cProtocolVersion{protocolVersion},
                                                                       mCallback{std::move(callback)}
        {
            bool _successful{mClient.TrySetup()};
            if (!_successful)
            {
                throw std::runtime_error("TCP client setup failed.");
            }

            _successful = mClient.TryConnect();
            if (!_successful)
            {
                throw std::runtime_error("TCP connection to the server failed.");
            }

            auto _onSend{std::bind(&DoipClient::onSend, this)};
            _successful = mPoller->TryAddSender(&mClient, _onSend);
            if (!_successful)
            {
                throw std::runtime_error("Send delegate register to the poller failed.");
            }

            auto _onReceive{std::bind(&DoipClient::onReceive, this)};
            _successful = mPoller->TryAddReceiver(&mClient, _onReceive);
            if (!_successful)
            {
                mPoller->TryRemoveSender(&mClient);
                throw std::runtime_error("Receive delegate register to the poller failed.");
            }

            const DoipLib::VehicleIdRequest cRequest(cProtocolVersion);
            std::vector<uint8_t> _serializedRequest;
            cRequest.Serialize(_serializedRequest);
            _successful = mSendQueue.TryEnqueue(std::move(_serializedRequest));
            if (!_successful)
            {
                throw std::runtime_error("Vehicle ID request enqueue failed");
            }
        }

        void DoipClient::handleVehicleIdResponse(
            const std::vector<uint8_t> &serializedMessage)
        {
            DoipLib::GenericNackType _nack;
            DoipLib::VehicleIdResponse _response;
            const bool cSuccessful{
                _response.TryDeserialize(serializedMessage, _nack)};

            if (cSuccessful)
            {
                mLogicalAddress = _response.GetLogicalAddress();
            }
        }

        void DoipClient::handleDiagMessageAck(
            const std::vector<uint8_t> &serializedMessage) const
        {
            DoipLib::GenericNackType _nack;
            DoipLib::DiagMessageAck _ackMessage;
            bool _successful{_ackMessage.TryDeserialize(serializedMessage, _nack)};

            if (_successful)
            {
                std::vector<uint8_t> _previousMessage;
                _successful = _ackMessage.TryGetPreviousMessage(_previousMessage);

                if (_successful)
                {
                    mCallback(std::move(_previousMessage));
                }
            }
        }

        void DoipClient::onSend()
        {
            while (!mSendQueue.Empty())
            {
                std::vector<uint8_t> _serializedMessageVec;
                const bool cDequeued{mSendQueue.TryDequeue(_serializedMessageVec)};

                if (cDequeued && _serializedMessageVec.size() <= cDoipPacketSize)
                {
                    auto _moveItr{
                        std::make_move_iterator(_serializedMessageVec.begin())};
                    std::array<uint8_t, cDoipPacketSize> _serializedMessageArr;

                    std::copy_n(
                        _moveItr,
                        _serializedMessageVec.size(),
                        _serializedMessageArr.begin());

                    mClient.Send(_serializedMessageArr);
                }

                std::this_thread::yield();
            }
        }

        void DoipClient::onReceive()
        {
            std::array<uint8_t, cDoipPacketSize> _receiveBuffer;
            mClient.Receive(_receiveBuffer);

            const std::vector<uint8_t> cSerializedMessage(
                _receiveBuffer.cbegin(), _receiveBuffer.cend());

            DoipLib::PayloadType _payloadType;
            const bool cSuccessful{
                DoipLib::Message::TryExtractPayloadType(
                    cSerializedMessage, _payloadType)};

            if (cSuccessful)
            {
                if (_payloadType ==
                    DoipLib::PayloadType::VehicleAnnoucementIdResponse)
                {
                    handleVehicleIdResponse(cSerializedMessage);
                }
                else if (_payloadType ==
                         DoipLib::PayloadType::DiagMessagePositiveAcknowledgement)
                {
                    handleDiagMessageAck(cSerializedMessage);
                }
            }
        }

        bool DoipClient::TrySendDiagMessage(std::vector<uint8_t> &&userData)
        {
            /// @note Sending diagnostic messages is possible only when the logical address is fetched.
            bool _result{mLogicalAddress.HasValue()};
            if (_result)
            {
                const DoipLib::DiagMessage cRequest(
                    cProtocolVersion,
                    mLogicalAddress.Value(),
                    mLogicalAddress.Value(),
                    std::move(userData));

                std::vector<uint8_t> _serializedRequest;
                cRequest.Serialize(_serializedRequest);
                _result = mSendQueue.TryEnqueue(std::move(_serializedRequest));
            }

            return _result;
        }

        DoipClient::~DoipClient()
        {
            mPoller->TryRemoveReceiver(&mClient);
            mPoller->TryRemoveSender(&mClient);
        }
    }
}