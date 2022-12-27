#include <algorithm>
#include "./socket_rpc_client.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                const size_t SocketRpcClient::cBufferSize;

                SocketRpcClient::SocketRpcClient(
                    AsyncBsdSocketLib::Poller *poller,
                    std::string ipAddress,
                    uint16_t port,
                    uint8_t protocolVersion,
                    uint8_t interfaceVersion) : RpcClient(protocolVersion, interfaceVersion),
                                                mPoller{poller},
                                                mClient{AsyncBsdSocketLib::TcpClient(ipAddress, port)}
                {
                    bool _successful{mClient.TrySetup()};
                    if (!_successful)
                    {
                        throw std::runtime_error(
                            "TCP client socket setup failed.");
                    }

                    _successful = mClient.TryConnect();
                    if (!_successful)
                    {
                        throw std::runtime_error(
                            "TCP client socket connection failed.");
                    }

                    auto _sender{std::bind(&SocketRpcClient::onSend, this)};
                    _successful = mPoller->TryAddSender(&mClient, _sender);
                    if (!_successful)
                    {
                        throw std::runtime_error(
                            "Adding TCP client socket sender failed.");
                    }

                    auto _receiver{std::bind(&SocketRpcClient::onReceive, this)};
                    _successful = mPoller->TryAddReceiver(&mClient, _receiver);
                    if (!_successful)
                    {
                        throw std::runtime_error(
                            "Adding TCP client socket receiver failed.");
                    }
                }

                void SocketRpcClient::onSend()
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

                            mClient.Send(_buffer);
                        }
                    }
                }

                void SocketRpcClient::onReceive()
                {
                    std::array<uint8_t, cBufferSize> _buffer;
                    ssize_t _receivedSize{mClient.Receive(_buffer)};

                    if (_receivedSize > 0)
                    {
                        const std::vector<uint8_t> cPayload(
                            std::make_move_iterator(_buffer.begin()),
                            std::make_move_iterator(_buffer.begin() + _receivedSize));

                        InvokeHandler(cPayload);
                    }
                }

                void SocketRpcClient::Send(const std::vector<uint8_t> &payload)
                {
                    mSendingQueue.TryEnqueue(payload);
                }

                SocketRpcClient::~SocketRpcClient()
                {
                    mPoller->TryRemoveReceiver(&mClient);
                    mPoller->TryRemoveSender(&mClient);
                }
            }
        }
    }
}