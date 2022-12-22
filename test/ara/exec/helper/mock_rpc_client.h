#ifndef MOCK_RPC_CLIENT_H
#define MOCK_RPC_CLIENT_H

#include "../../../../src/ara/com/someip/rpc/rpc_client.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            class MockRpcClient : public com::someip::rpc::RpcClient
            {
            public:
                MockRpcClient(
                    uint8_t protocolVersion = 1,
                    uint8_t interfaceVersion = 1) noexcept : com::someip::rpc::RpcClient(protocolVersion, interfaceVersion)
                {
                }

                void Send(const std::vector<uint8_t> &payload) override
                {
                    const com::someip::SomeIpReturnCode cReturnCode{
                        com::someip::SomeIpReturnCode::eOK};
                    const std::vector<uint8_t> cRpcPayload;
                    const com::someip::rpc::SomeIpRpcMessage cRequest{
                        com::someip::rpc::SomeIpRpcMessage::Deserialize(payload)};

                    const com::someip::rpc::SomeIpRpcMessage cResponse(
                        cRequest.MessageId(),
                        cRequest.ClientId(),
                        cRequest.SessionId(),
                        cRequest.ProtocolVersion(),
                        cRequest.InterfaceVersion(),
                        cReturnCode,
                        cRpcPayload);

                    const std::vector<uint8_t> cResponsePayload{cResponse.Payload()};
                    InvokeHandler(cResponsePayload);
                }
            };
        }
    }
}

#endif