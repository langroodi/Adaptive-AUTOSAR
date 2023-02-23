#include "./rpc_configuration.h"

namespace application
{
    namespace helper
    {
        bool TryGetRpcConfiguration(
            const std::string &configFilepath,
            std::string networkEndpoint,
            std::string applicationEndpoint,
            RpcConfiguration &configuration)
        {
            const arxml::ArxmlReader cArxmlReader(configFilepath);
            const ara::com::option::Layer4ProtocolType cProtocol{
                ara::com::option::Layer4ProtocolType::Tcp};

            TryGetNetworkConfiguration(
                cArxmlReader,
                networkEndpoint,
                applicationEndpoint,
                cProtocol,
                configuration);

            uint8_t _protocolVersion;
            if (!TryExtractDeepValue(
                    cArxmlReader,
                    cProtocolVersionShallowChildren,
                    std::array<std::string, 0>{},
                    "",
                    _protocolVersion))
            {
                return false;
            }

            configuration.protocolVersion = _protocolVersion;

            return true;
        }
    }
}