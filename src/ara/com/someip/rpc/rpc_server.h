#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include <functional>
#include <map>
#include <set>
#include <stdint.h>
#include <vector>

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
                    uint8_t mProtocolVersion;
                    uint8_t mInterfaceVersion;
                    std::set<uint16_t> mServices;
                    std::map<uint32_t, HandlerType> mHandlers;

                protected:
                    /// @brief Constructor
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    RpcServer(uint8_t protocolVersion, uint8_t interfaceVersion);

                    /// @brief Invoke corresponding request handler at a message reception
                    /// @param[in] requestPayload Serialized SOME/IP request payload byte vector
                    /// @param[out] responsePayload Serialized SOME/IP response payload byte vector
                    void InvokeHandler(
                        const std::vector<uint8_t> &requestPayload,
                        std::vector<uint8_t> &responsePayload);

                public:
                    RpcServer() = delete;
                    RpcServer(const RpcServer &) = delete;

                    /// @brief Set a RPC request handler
                    /// @param serviceId Service ID that contains the requested method
                    /// @param methodId Requested method ID for invocation
                    /// @param handler Handler to be invoked at the request arrival
                    void SetHandler(
                        uint16_t serviceId, uint16_t methodId, HandlerType handler);
                };
            }
        }
    }
}

#endif