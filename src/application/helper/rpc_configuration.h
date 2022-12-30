#ifndef RPC_CONFIGURATION_H
#define RPC_CONFIGURATION_H

#include <map>
#include <string>
#include "../../arxml/arxml_reader.h"

namespace application
{
        static const std::initializer_list<std::string> cIpAddressShallowChildren(
            {"AUTOSAR",
             "AR-PACKAGES",
             "AR-PACKAGE",
             "ELEMENTS",
             "COMMUNICATION-CLUSTER",
             "ETHERNET-PHYSICAL-CHANNEL",
             "NETWORK-ENDPOINTS"});

        static const std::initializer_list<std::string> cIpAddressDeepChildren(
            {"NETWORK-ENDPOINT",
             "NETWORK-ENDPOINT-ADDRESSES",
             "IPV-4-CONFIGURATION",
             "IPV-4-ADDRESS"});

        static const std::initializer_list<std::string> cPortNumberShallowChildren(
            {"AUTOSAR",
             "AR-PACKAGES",
             "AR-PACKAGE",
             "ELEMENTS",
             "ETHERNET-COMMUNICATION-CONNECTOR",
             "AP-APPLICATION-ENDPOINTS"});

        static const std::initializer_list<std::string> cPortNumberDeepChildren(
            {"AP-APPLICATION-ENDPOINT",
             "TP-CONFIGURATION",
             "TCP-TP",
             "TCP-TP-PORT",
             "PORT-NUMBER"});

        static const std::initializer_list<std::string> cProtocolVersionShallowChildren(
            {"AUTOSAR",
             "AR-PACKAGES",
             "AR-PACKAGE",
             "ELEMENTS",
             "COMMUNICATION-CLUSTER",
             "PROTOCOL-VERSION"});

    /// @brief Namespace that contains the helper classes for the application
    namespace helper
    {
        /// @brief Data model for a remote procedure call connection configuration
        struct RpcConfiguration
        {
            /// @brief RPC server IPv4 address
            std::string ipAddress;

            /// @brief RPC server TCP listen port number
            uint16_t portNumber;

            /// @brief SOME/IP RPC version
            uint8_t protocolVersion;
        };

        template <typename T>
        static bool tryExtractDeepValue(
            const arxml::ArxmlReader &shallowReader,
            std::initializer_list<std::string> shallowChildren,
            std::initializer_list<std::string> deepChildren,
            std::string shortNameFilter,
            T &deepValue)
        {
            const arxml::ArxmlNodeRange cShallowNodes{
                shallowReader.GetNodes(shallowChildren)};

            for (const auto cShallowNode : cShallowNodes)
            {
                if (shortNameFilter.empty())
                {
                    deepValue = cShallowNode.GetValue<T>();
                    return true;
                }

                std::string _shortName{cShallowNode.GetShortName()};
                if (_shortName == shortNameFilter)
                {
                    std::string _content{cShallowNode.GetContent()};
                    arxml::ArxmlReader _deepReader(_content.c_str(), _content.length());
                    arxml::ArxmlNode _deepNode{_deepReader.GetRootNode(deepChildren)};
                    deepValue = _deepNode.GetValue<T>();

                    return true;
                }
            }

            return false;
        }

        /// @brief Try to get a RPC configuration based on a ARXML configuration file
        /// @param[in] configFilepath ARXML configuration file path
        /// @param[in] networkEndpoint Desired network endpoint name
        /// @param[in] applicationEndpoint Desired application endpoint name
        /// @param[out] rpcConfiguration Filled RPC configuration
        /// @return True if the configuration is filled properly; otherwise false
        bool TryGetCRpcConfiguration(
            const std::string &configFilepath,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            RpcConfiguration &rpcConfiguration);
    }
}

#endif