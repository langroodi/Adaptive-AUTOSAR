#include <utility>
#include "./rpc_server.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                RpcServer::RpcServer(
                    uint8_t protocolVersion, uint8_t interfaceVersion) noexcept : mProtocolVersion{protocolVersion},
                                                                                  mInterfaceVersion{interfaceVersion}
                {
                }

                SomeIpReturnCode RpcServer::validate(
                    const SomeIpRpcMessage &request) const
                {
                    if (request.MessageType() != SomeIpMessageType::Request)
                    {
                        return SomeIpReturnCode::eWrongMessageType;
                    }

                    if (request.ProtocolVersion() != mProtocolVersion)
                    {
                        return SomeIpReturnCode::eWrongProtocolVersion;
                    }

                    if (request.InterfaceVersion() != mInterfaceVersion)
                    {
                        return SomeIpReturnCode::eWrongInterfaceVersion;
                    }

                    auto _serviceId{static_cast<uint16_t>(request.MessageId() >> 16)};
                    if (mServices.find(_serviceId) == mServices.end())
                    {
                        return SomeIpReturnCode::eUnknownService;
                    }

                    return SomeIpReturnCode::eOK;
                }

                void RpcServer::getResponsePayload(
                    const SomeIpRpcMessage &request,
                    SomeIpReturnCode returnCode,
                    const std::vector<uint8_t> &rpcPayload,
                    std::vector<uint8_t> &payload) const
                {
                    SomeIpRpcMessage _errorMessage(
                        request.MessageId(),
                        request.ClientId(),
                        request.SessionId(),
                        mProtocolVersion,
                        mInterfaceVersion,
                        returnCode,
                        rpcPayload);

                    payload = std::move(_errorMessage.Payload());
                }

                void RpcServer::getResponsePayload(
                    const SomeIpRpcMessage &request,
                    SomeIpReturnCode returnCode,
                    std::vector<uint8_t> &payload) const
                {
                    const std::vector<uint8_t> cRpcPayload;
                    getResponsePayload(request, returnCode, cRpcPayload, payload);
                }

                bool RpcServer::TryInvokeHandler(
                    const std::vector<uint8_t> &requestPayload,
                    std::vector<uint8_t> &responsePayload) const
                {
                    try
                    {
                        const SomeIpRpcMessage _request{
                            SomeIpRpcMessage::Deserialize(requestPayload)};

                        SomeIpReturnCode _returnCode{validate(_request)};
                        if (_returnCode != SomeIpReturnCode::eOK)
                        {
                            getResponsePayload(
                                _request, _returnCode,
                                responsePayload);

                            return true;
                        }

                        auto _itr{mHandlers.find(_request.MessageId())};
                        if (_itr != mHandlers.end())
                        {
                            std::vector<uint8_t> _rpcResponsePdu;
                            bool _handled{
                                _itr->second(_request.RpcPayload(), _rpcResponsePdu)};

                            if (_handled)
                            {
                                getResponsePayload(
                                    _request, SomeIpReturnCode::eOK, _rpcResponsePdu,
                                    responsePayload);
                            }
                            else
                            {
                                getResponsePayload(
                                    _request, SomeIpReturnCode::eNotOk, _rpcResponsePdu,
                                    responsePayload);
                            }
                        }
                        else
                        {
                            getResponsePayload(
                                _request, SomeIpReturnCode::eUnknownMethod,
                                responsePayload);
                        }

                        return true;
                    }
                    catch (std::out_of_range)
                    {
                        return false;
                    }
                }

                void RpcServer::SetHandler(
                    uint16_t serviceId, uint16_t methodId, HandlerType handler)
                {
                    auto _messageId{static_cast<uint32_t>(serviceId << 16)};
                    _messageId |= methodId;
                    mHandlers[_messageId] = handler;

                    mServices.insert(serviceId);
                }
            }
        }
    }
}