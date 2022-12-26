#ifndef MOCK_RPC_SERVER_H
#define MOCK_RPC_SERVER_H

#include <utility>
#include "../../../../src/ara/com/someip/rpc/rpc_server.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            class MockRpcServer : public com::someip::rpc::RpcServer
            {
            public:
                MockRpcServer(
                    uint8_t protocolVersion,
                    uint8_t interfaceVersion) noexcept : com::someip::rpc::RpcServer(protocolVersion, interfaceVersion)
                {
                }

                com::someip::rpc::SomeIpRpcMessage Send(
                    const com::someip::rpc::SomeIpRpcMessage &request)
                {
                    const std::vector<uint8_t> cRequestPayload{request.Payload()};
                    std::vector<uint8_t> _responsePayload;

                    bool _handled{TryInvokeHandler(cRequestPayload, _responsePayload)};
                    if (_handled)
                    {
                        auto _result{
                            com::someip::rpc::SomeIpRpcMessage::Deserialize(
                                _responsePayload)};

                        return _result;
                    }
                    else
                    {
                        throw std::logic_error(
                            "Request handling is failed at the server side.");
                    }
                }

                static bool TryGetErrorCode(
                    const com::someip::rpc::SomeIpRpcMessage &message,
                    ExecErrc &errorCode)
                {
                    const size_t cPayloadSize{sizeof(uint32_t)};
                    const auto cMin{static_cast<uint32_t>(ExecErrc::kGeneralError)};
                    const auto cMax{static_cast<uint32_t>(ExecErrc::kCycleOverrun)};

                    if (message.MessageType() != com::someip::SomeIpMessageType::Error)
                    {
                        return false;
                    }

                    if (message.RpcPayload().size() != cPayloadSize)
                    {
                        return false;
                    }

                    size_t _offset{0};
                    uint32_t _errorCodeInt{
                        com::helper::ExtractInteger(message.RpcPayload(), _offset)};
                    if (_errorCodeInt < cMin || _errorCodeInt > cMax)
                    {
                        return false;
                    }

                    errorCode = static_cast<ExecErrc>(_errorCodeInt);
                    return true;
                }
            };
        }
    }
}

#endif