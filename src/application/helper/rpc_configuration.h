#ifndef RPC_CONFIGURATION_H
#define RPC_CONFIGURATION_H


#include "./network_configuration.h"

namespace application
{
    /// @brief Namespace that contains the helper classes for the application
    namespace helper
    {
        static const std::array<std::string, 6> cProtocolVersionShallowChildren(
            {"AUTOSAR",
             "AR-PACKAGES",
             "AR-PACKAGE",
             "ELEMENTS",
             "COMMUNICATION-CLUSTER",
             "PROTOCOL-VERSION"});

        /// @brief Data model for a remote procedure call connection configuration
        struct RpcConfiguration : public NetworkConfiguration
        {
            /// @brief SOME/IP RPC version
            uint8_t protocolVersion;
        };

        /// @brief Try to get a RPC configuration based on a ARXML configuration file
        /// @param[in] configFilepath ARXML configuration file path
        /// @param[in] networkEndpoint Desired network endpoint name
        /// @param[in] applicationEndpoint Desired application endpoint name
        /// @param[out] configuration Filled RPC configuration
        /// @return True if the configuration is filled properly; otherwise false
        bool TryGetRpcConfiguration(
            const std::string &configFilepath,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            RpcConfiguration &configuration);
    }
}

#endif