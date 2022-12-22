#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H

#include <functional>
#include <map>
#include <stdint.h>
#include <vector>
#include "./someip_rpc_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                /// @brief SOME/IP RPC abstract client
                class RpcClient
                {
                public:
                    /// @brief SOME/IP RPC response handler type
                    using HandlerType = std::function<void(const SomeIpRpcMessage &)>;

                private:
                    const uint8_t mProtocolVersion;
                    const uint8_t mInterfaceVersion;
                    std::map<uint32_t, uint16_t> mSessionIds;
                    std::map<uint32_t, HandlerType> mHandlers;

                protected:
                    /// @brief Constructor
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    RpcClient(
                        uint8_t protocolVersion,
                        uint8_t interfaceVersion) noexcept;

                    /// @brief Invoke corresponding response handler at a message reception
                    /// @param payload Serialized SOME/IP response payload byte vector
                    void InvokeHandler(const std::vector<uint8_t> &payload) const;

                    /// @brief Send a SOME/IP request to the RPC server
                    /// @param payload Serialized SOME/IP request payload byte vector
                    virtual void Send(const std::vector<uint8_t> &payload) = 0;

                public:
                    /// @brief Set a RPC response handler
                    /// @param serviceId Service ID that contains the requested method
                    /// @param methodId Requested method ID for invocation
                    /// @param handler Handler to be invoked at the response arrival
                    void SetHandler(
                        uint16_t serviceId, uint16_t methodId, HandlerType handler);

                    /// @brief Send a request to the RPC server
                    /// @param serviceId Service ID that contains the requested method
                    /// @param methodId Requested method ID for invocation
                    /// @param clientId Client ID that sends the request
                    /// @param rpcPayload Serialized RPC request payload byte vector
                    void Send(
                        uint16_t serviceId,
                        uint16_t methodId,
                        uint16_t clientId,
                        const std::vector<uint8_t> &rpcPayload);

                    virtual ~RpcClient() noexcept = default;
                };
            }
        }
    }
}

#endif