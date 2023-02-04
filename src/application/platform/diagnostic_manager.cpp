#include "../../ara/com/someip/sd/sd_network_layer.h"
#include "./diagnostic_manager.h"

namespace application
{
    namespace platform
    {

        const std::string DiagnosticManager::cAppId{"DiagnosticManager"};

        DiagnosticManager::DiagnosticManager(AsyncBsdSocketLib::Poller *poller) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                                                                  mNetworkLayer{nullptr},
                                                                                  mSdClient{nullptr}
        {
        }

        void DiagnosticManager::configureNetworkLayer(const arxml::ArxmlReader &reader)
        {
            const std::string cNicIpAddress{"127.0.0.1"};
            const std::string cNetworkEndpoint{"DiagnosticManagerEP"};
            const std::string cApplicationEndpoint{"MulticastUdp"};
            const ara::com::option::Layer4ProtocolType cProtocol{
                ara::com::option::Layer4ProtocolType::Udp};

            helper::NetworkConfiguration _networkConfiguration;
            bool _successful{
                helper::TryGetNetworkConfiguration(
                    reader, cNetworkEndpoint, cApplicationEndpoint, cProtocol,
                    _networkConfiguration)};

            if (_successful)
            {
                mNetworkLayer =
                    new ara::com::someip::sd::SdNetworkLayer(
                        Poller, cNicIpAddress,
                        _networkConfiguration.ipAddress,
                        _networkConfiguration.portNumber);
            }
            else
            {
                throw std::runtime_error("Fetching network configuration failed.");
            }
        }

        void DiagnosticManager::configureSdClient(const arxml::ArxmlReader &reader)
        {
            const arxml::ArxmlNode cServiceIdNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "REQUIRED-SOMEIP-SERVICE-INSTANCE",
                                    "SERVICE-INTERFACE-DEPLOYMENT",
                                    "SERVICE-INTERFACE-ID"})};

            const arxml::ArxmlNode cInitialDelayMinNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "REQUIRED-SOMEIP-SERVICE-INSTANCE",
                                    "SD-CLIENT-CONFIG",
                                    "INITIAL-FIND-BEHAVIOR",
                                    "INITIAL-DELAY-MIN-VALUE"})};

            const arxml::ArxmlNode cInitialDelayMaxNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "REQUIRED-SOMEIP-SERVICE-INSTANCE",
                                    "SD-CLIENT-CONFIG",
                                    "INITIAL-FIND-BEHAVIOR",
                                    "INITIAL-DELAY-MAX-VALUE"})};

            const auto cServiceId{cServiceIdNode.GetValue<uint16_t>()};
            const auto cInitialDelayMin{cInitialDelayMinNode.GetValue<int>()};
            const auto cInitialDelayMax{cInitialDelayMaxNode.GetValue<int>()};

            mSdClient =
                new ara::com::someip::sd::SomeIpSdClient(
                    mNetworkLayer,
                    cServiceId,
                    cInitialDelayMin,
                    cInitialDelayMax);
        }

        int DiagnosticManager::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const std::string cDmConfigArgument{"dmconfig"};
            const std::string cDmConfigFilepath{arguments.at(cDmConfigArgument)};
            const arxml::ArxmlReader cReader(cDmConfigFilepath);

            ara::log::LogStream _logStream;

            try
            {
                configureNetworkLayer(cReader);
                configureSdClient(cReader);

                _logStream << "Diagnostic Manager has been initialized.";
                Log(cLogLevel, _logStream);

                bool _running{true};
                mSdClient->Start();

                while (!cancellationToken->load() && _running)
                {
                    _running = WaitForActivation();
                }

                delete mSdClient;
                mSdClient = nullptr;

                _logStream.Flush();
                _logStream << "Diagnostic Manager has been terminated.";
                Log(cLogLevel, _logStream);

                return cSuccessfulExitCode;
            }
            catch (const std::runtime_error &ex)
            {
                _logStream.Flush();
                _logStream << ex.what();
                Log(cErrorLevel, _logStream);

                return cUnsuccessfulExitCode;
            }
        }

        DiagnosticManager::~DiagnosticManager()
        {
            if (mSdClient)
                delete mSdClient;

            if (mNetworkLayer)
                delete mNetworkLayer;
        }
    }
}