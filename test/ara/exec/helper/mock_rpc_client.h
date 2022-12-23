#ifndef MOCK_RPC_CLIENT_H
#define MOCK_RPC_CLIENT_H

#include <utility>
#include "../../../../src/ara/com/someip/rpc/rpc_client.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            class MockRpcClient : public com::someip::rpc::RpcClient
            {
            private:
                bool mBypass;
                std::vector<uint8_t> mRpcPayload;

            public:
                MockRpcClient(
                    uint8_t protocolVersion = 1,
                    uint8_t interfaceVersion = 1) noexcept : com::someip::rpc::RpcClient(protocolVersion, interfaceVersion),
                                                             mBypass{false}
                {
                }

                void SetBypass(bool bypass)
                {
                    mBypass = bypass;
                }

                void SetRpcPayload(std::vector<uint8_t> &&rpcPayload)
                {
                    mRpcPayload = std::move(rpcPayload);
                }

                void Send(const std::vector<uint8_t> &payload) override
                {
                    if (!mBypass)
                    {
                        const com::someip::SomeIpReturnCode cReturnCode{
                            mRpcPayload.empty() ? com::someip::SomeIpReturnCode::eOK : com::someip::SomeIpReturnCode::eNotOk};
                        const com::someip::rpc::SomeIpRpcMessage cRequest{
                            com::someip::rpc::SomeIpRpcMessage::Deserialize(payload)};

                        const com::someip::rpc::SomeIpRpcMessage cResponse(
                            cRequest.MessageId(),
                            cRequest.ClientId(),
                            cRequest.SessionId(),
                            cRequest.ProtocolVersion(),
                            cRequest.InterfaceVersion(),
                            cReturnCode,
                            mRpcPayload);

                        const std::vector<uint8_t> cResponsePayload{cResponse.Payload()};
                        InvokeHandler(cResponsePayload);
                    }
                }
            };
        }
    }
}

#endif