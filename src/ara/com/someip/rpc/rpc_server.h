#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include <functional>
#include <map>
#include <set>
#include <stdint.h>
#include <vector>
#include "./someip_rpc_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            /// @brief SOME/IP Remote Procedure Call namespace
            /// @note The namespace is NOT part of the Adaptive AUTOSAR standard.
            namespace rpc
            {
                /// @brief SOME/IP RPC server
                class RpcServer
                {
                public:
                    /// @brief SOME/IP RPC request handler type
                    using HandlerType =
                        std::function<bool(const std::vector<uint8_t> &, std::vector<uint8_t> &)>;

                private:
                    const uint8_t mProtocolVersion;
                    const uint8_t mInterfaceVersion;
                    std::set<uint16_t> mServices;
                    std::map<uint32_t, HandlerType> mHandlers;

                    SomeIpReturnCode validate(
                        const SomeIpRpcMessage &request) const;

                    void getResponsePayload(
                        const SomeIpRpcMessage &request,
                        SomeIpReturnCode returnCode,
                        const std::vector<uint8_t>& rpcPayload,
                        std::vector<uint8_t> &payload) const;

                    void getResponsePayload(
                        const SomeIpRpcMessage &request,
                        SomeIpReturnCode returnCode,
                        std::vector<uint8_t> &payload) const;

                protected:
                    /// @brief Constructor
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    RpcServer(
                        uint8_t protocolVersion, uint8_t interfaceVersion) noexcept;

                    /// @brief Try to invoke corresponding request handler at a message reception
                    /// @param[in] requestPayload Serialized SOME/IP request payload byte vector
                    /// @param[out] responsePayload Serialized SOME/IP response payload byte vector
                    /// @returns True if the request is handled; otherwise false
                    /// @remark The response payload will be untouched if the request is NOT handled.
                    bool TryInvokeHandler(
                        const std::vector<uint8_t> &requestPayload,
                        std::vector<uint8_t> &responsePayload) const;

                public:
                    RpcServer() = delete;
                    RpcServer(const RpcServer &) = delete;

                    /// @brief Set a RPC request handler
                    /// @param serviceId Service ID that contains the requested method
                    /// @param methodId Requested method ID for invocation
                    /// @param handler Handler to be invoked at the request arrival
                    void SetHandler(
                        uint16_t serviceId, uint16_t methodId, HandlerType handler);

                    virtual ~RpcServer() noexcept = default;
                };
            }
        }
    }
}

#endif