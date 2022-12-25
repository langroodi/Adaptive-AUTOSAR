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
            };
        }
    }
}

#endif