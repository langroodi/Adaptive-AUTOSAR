#include <algorithm>
#include "./sd_network_layer.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                const size_t SdNetworkLayer::cBufferSize{256};
                const std::string SdNetworkLayer::cAnyIpAddress("0.0.0.0");

                SdNetworkLayer::SdNetworkLayer(
                    AsyncBsdSocketLib::Poller *poller,
                    std::string nicIpAddress,
                    std::string multicastGroup,
                    uint16_t port) : cNicIpAddress{nicIpAddress},
                                     cMulticastGroup{multicastGroup},
                                     cPort{port},
                                     mPoller{poller},
                                     mUdpSocket(cAnyIpAddress, port, nicIpAddress, multicastGroup)
                {
                    bool _successful{mUdpSocket.TrySetup()};
                    if (!_successful)
                    {
                        throw std::runtime_error("UDP socket setup failed.");
                    }

                    auto _receiver{std::bind(&SdNetworkLayer::onReceive, this)};
                    _successful = mPoller->TryAddReceiver(&mUdpSocket, _receiver);
                    if (!_successful)
                    {
                        throw std::runtime_error("Adding UDP socket receiver failed.");
                    }

                    auto _sender{std::bind(&SdNetworkLayer::onSend, this)};
                    _successful = mPoller->TryAddSender(&mUdpSocket, _sender);
                    if (!_successful)
                    {
                        throw std::runtime_error("Adding UDP socket sender failed.");
                    }
                }

                void SdNetworkLayer::onReceive()
                {
                    std::array<uint8_t, cBufferSize> _buffer;
                    std::string _ipAddress;
                    uint16_t _port;
                    ssize_t _receivedSize{
                        mUdpSocket.Receive(_buffer, _ipAddress, _port)};

                    if (_receivedSize > 0 &&
                        _port == cPort && _ipAddress == cNicIpAddress)
                    {
                        const std::vector<uint8_t> cRequestPayload(
                            std::make_move_iterator(_buffer.begin()),
                            std::make_move_iterator(_buffer.begin() + _receivedSize));

                        FireReceiverCallbacks(cRequestPayload);
                    }
                }

                void SdNetworkLayer::onSend()
                {
                    while (!mSendingQueue.Empty())
                    {
                        std::vector<uint8_t> _payload;
                        bool _dequeued{mSendingQueue.TryDequeue(_payload)};
                        if (_dequeued)
                        {
                            std::array<uint8_t, cBufferSize> _buffer;
                            std::copy_n(
                                std::make_move_iterator(_payload.begin()),
                                _payload.size(),
                                _buffer.begin());

                            mUdpSocket.Send(_buffer, cMulticastGroup, cPort);
                        }
                    }
                }

                void SdNetworkLayer::Send(const SomeIpSdMessage &message)
                {
                    std::vector<uint8_t> _payload{message.Payload()};
                    mSendingQueue.TryEnqueue(std::move(_payload));
                }

                SdNetworkLayer::~SdNetworkLayer()
                {
                    mPoller->TryRemoveSender(&mUdpSocket);
                    mPoller->TryRemoveReceiver(&mUdpSocket);
                }
            }
        }
    }
}