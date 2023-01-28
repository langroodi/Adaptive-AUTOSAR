#include "./network_configuration.h"

namespace application
{
    namespace helper
    {
        bool TryGetNetworkConfiguration(
            const arxml::ArxmlReader &reader,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            ara::com::option::Layer4ProtocolType protocol,
            NetworkConfiguration &configuration)
        {
            std::string _ipAddress;
            if (!TryExtractDeepValue(
                    reader,
                    cIpAddressShallowChildren,
                    cIpAddressDeepChildren,
                    networkEndpoint,
                    _ipAddress))
            {
                return false;
            }

            uint16_t _portNumber;
            switch (protocol)
            {
            case ara::com::option::Layer4ProtocolType::Tcp:
                if (!TryExtractDeepValue(
                        reader,
                        cPortNumberShallowChildren,
                        cTcpPortNumberDeepChildren,
                        applicationEndpoint,
                        _portNumber))
                {
                    return false;
                }
                break;

            case ara::com::option::Layer4ProtocolType::Udp:
                if (!TryExtractDeepValue(
                        reader,
                        cPortNumberShallowChildren,
                        cUdpPortNumberDeepChildren,
                        applicationEndpoint,
                        _portNumber))
                {
                    return false;
                }
                break;

            default:
                return false;
            }

            configuration.ipAddress = _ipAddress;
            configuration.portNumber = _portNumber;

            return true;
        }

        bool TryGetNetworkConfiguration(
            const std::string &configFilepath,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            ara::com::option::Layer4ProtocolType protocol,
            NetworkConfiguration &configuration)
        {
            const arxml::ArxmlReader cArxmlReader(configFilepath);
            bool _result{
                TryGetNetworkConfiguration(
                    cArxmlReader,
                    networkEndpoint,
                    applicationEndpoint,
                    protocol,
                    configuration)};

            return _result;
        }
    }
}