#ifndef NETWORK_CONFIGURATION_H
#define NETWORK_CONFIGURATION_H

#include <map>
#include <string>
#include "../../arxml/arxml_reader.h"
#include "../../ara/com/option/option.h"

namespace application
{
    namespace helper
    {
        static const std::array<std::string, 7> cIpAddressShallowChildren(
            {"AUTOSAR",
             "AR-PACKAGES",
             "AR-PACKAGE",
             "ELEMENTS",
             "COMMUNICATION-CLUSTER",
             "ETHERNET-PHYSICAL-CHANNEL",
             "NETWORK-ENDPOINTS"});

        static const std::array<std::string, 4> cIpAddressDeepChildren(
            {"NETWORK-ENDPOINT",
             "NETWORK-ENDPOINT-ADDRESSES",
             "IPV-4-CONFIGURATION",
             "IPV-4-ADDRESS"});

        static const std::array<std::string, 6> cPortNumberShallowChildren(
            {"AUTOSAR",
             "AR-PACKAGES",
             "AR-PACKAGE",
             "ELEMENTS",
             "ETHERNET-COMMUNICATION-CONNECTOR",
             "AP-APPLICATION-ENDPOINTS"});

        static const std::array<std::string, 5> cTcpPortNumberDeepChildren(
            {"AP-APPLICATION-ENDPOINT",
             "TP-CONFIGURATION",
             "TCP-TP",
             "TCP-TP-PORT",
             "PORT-NUMBER"});

        static const std::array<std::string, 5> cUdpPortNumberDeepChildren(
            {"AP-APPLICATION-ENDPOINT",
             "TP-CONFIGURATION",
             "UDP-TP",
             "UDP-TP-PORT",
             "PORT-NUMBER"});

        /// @brief Data model for a network configuration
        struct NetworkConfiguration
        {
            /// @brief Netowrk IPv4 address
            std::string ipAddress;

            /// @brief Network TCP/UDP port number
            uint16_t portNumber;
        };

        /// @brief Try to extract a deep value from an XML content
        /// @tparam T Deep value type
        /// @tparam M Shallow children depth level
        /// @tparam N Deep children depth level
        /// @param[in] shallowReader XML content reader
        /// @param[in] shallowChildren XML node hierarchy that may contain multiple deep values
        /// @param[in] deepChildren XML node hierarchy of a deep value
        /// @param[in] shortNameFilter Deep value short name for filtering within multiple deep values
        /// @param[out] deepValue Filled deep value
        /// @return True if the value is extracted properly; otherwise false
        template <typename T, size_t M, size_t N>
        bool TryExtractDeepValue(
            const arxml::ArxmlReader &shallowReader,
            const std::array<std::string, M> &shallowChildren,
            const std::array<std::string, N> &deepChildren,
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

        /// @brief Try to get a nework configuration based on a ARXML configuration file
        /// @param[in] reader ARXML configuration reader
        /// @param[in] networkEndpoint Desired network endpoint name
        /// @param[in] applicationEndpoint Desired application endpoint name
        /// @param[in] protocol Network transmission protocol
        /// @param[out] configuration Filled network configuration
        /// @return True if the configuration is filled properly; otherwise false
        bool TryGetNetworkConfiguration(
            const arxml::ArxmlReader &reader,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            ara::com::option::Layer4ProtocolType protocol,
            NetworkConfiguration &configuration);

        /// @brief Try to get a nework configuration based on a ARXML configuration file
        /// @param[in] configFilepath ARXML configuration file path
        /// @param[in] networkEndpoint Desired network endpoint name
        /// @param[in] applicationEndpoint Desired application endpoint name
        /// @param[in] protocol Network transmission protocol
        /// @param[out] configuration Filled network configuration
        /// @return True if the configuration is filled properly; otherwise false
        bool TryGetNetworkConfiguration(
            const std::string &configFilepath,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            ara::com::option::Layer4ProtocolType protocol,
            NetworkConfiguration &configuration);
    }

}

#endif