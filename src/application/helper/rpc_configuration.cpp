#include "./rpc_configuration.h"

namespace application
{
    namespace helper
    {
        bool TryGetCRpcConfiguration(
            const std::string &configFilepath,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            RpcConfiguration &rpcConfiguration)
        {
            const arxml::ArxmlReader cArxmlReader(configFilepath);

            std::string _ipAddress;
            if (!tryExtractDeepValue(
                    cArxmlReader,
                    cIpAddressShallowChildren,
                    cIpAddressDeepChildren,
                    networkEndpoint,
                    _ipAddress))
            {
                return false;
            }

            uint16_t _portNumber;
            if (!tryExtractDeepValue(
                    cArxmlReader,
                    cPortNumberShallowChildren,
                    cPortNumberDeepChildren,
                    applicationEndpoint,
                    _portNumber))
            {
                return false;
            }

            uint8_t _protocolVersion;
            if (!tryExtractDeepValue(
                    cArxmlReader,
                    cProtocolVersionShallowChildren,
                    {},
                    "",
                    _protocolVersion))
            {
                return false;
            }

            rpcConfiguration.ipAddress = _ipAddress;
            rpcConfiguration.portNumber = _portNumber;
            rpcConfiguration.protocolVersion = _protocolVersion;

            return true;
        }
    }
}