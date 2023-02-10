#include "../../ara/com/someip/sd/sd_network_layer.h"
#include "./diagnostic_manager.h"

namespace application
{
    namespace platform
    {

        const std::string DiagnosticManager::cAppId{"DiagnosticManager"};

        DiagnosticManager::DiagnosticManager(AsyncBsdSocketLib::Poller *poller) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                                                                  mNetworkLayer{nullptr},
                                                                                  mSdClient{nullptr},
                                                                                  mEvent{nullptr},
                                                                                  mMonitor{nullptr}
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

        void DiagnosticManager::configureEvent(const arxml::ArxmlReader &reader)
        {
            const arxml::ArxmlNode cEventNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "DIAGNOSTIC-EVENT-INTERFACE"})};

            const arxml::ArxmlNode cDtcNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "DIAGNOSTIC-EVENT-INTERFACE",
                                    "DTC-NUMBER"})};

            const std::string cShortName{cEventNode.GetShortName()};
            mEventSpecifier = new ara::core::InstanceSpecifier(cShortName);
            mEvent = new ara::diag::Event(*mEventSpecifier);

            const auto cDtcNumber{cDtcNode.GetValue<uint32_t>()};
            mEvent->SetDTCNumber(cDtcNumber);
        }

        void DiagnosticManager::onInitMonitor(ara::diag::InitMonitorReason reason)
        {
            ara::log::LogStream _logStream;

            switch (reason)
            {
            case ara::diag::InitMonitorReason::kReenabled:
                _logStream << mMonitorSpecifier << " is offered.";

                break;

            case ara::diag::InitMonitorReason::kDisabled:
                _logStream << mMonitorSpecifier << " offer is stopped.";
                break;

            default:
                auto _reasonInt{static_cast<uint32_t>(reason)};
                _logStream << mMonitorSpecifier << "'s reason is " << _reasonInt;
                break;
            }

            Log(cLogLevel, _logStream);
        }

        void DiagnosticManager::configureMonitor(const arxml::ArxmlReader &reader)
        {
            const arxml::ArxmlNode cMonitorNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "DIAGNOSTIC-MONITOR-INTERFACE"})};

            const arxml::ArxmlNode cTimePassedNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "DIAGNOSTIC-MONITOR-INTERFACE",
                                    "DIAG-EVENT-DEBOUNCE-TIME-BASED",
                                    "TIME-PASSED-THRESHOLD"})};

            const arxml::ArxmlNode cTimeFailedNode{
                reader.GetRootNode({"AUTOSAR",
                                    "AR-PACKAGES",
                                    "AR-PACKAGE",
                                    "ELEMENTS",
                                    "DIAGNOSTIC-MONITOR-INTERFACE",
                                    "DIAG-EVENT-DEBOUNCE-TIME-BASED",
                                    "TIME-FAILED-THRESHOLD"})};

            const std::string cShortName{cMonitorNode.GetShortName()};
            mMonitorSpecifier = new ara::core::InstanceSpecifier(cShortName);

            ara::diag::TimeBased _timeBased;
            _timeBased.passedMs = cTimePassedNode.GetValue<uint32_t>();
            _timeBased.failedMs = cTimeFailedNode.GetValue<uint32_t>();

            auto _initMonitor{
                std::bind(
                    &DiagnosticManager::onInitMonitor,
                    this, std::placeholders::_1)};

            mMonitor =
                new ara::diag::Monitor(
                    *mMonitorSpecifier, _initMonitor, _timeBased);
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
                configureEvent(cReader);
                configureMonitor(cReader);

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
            if (mMonitor)
            {
                delete mMonitorSpecifier;
                delete mMonitor;
            }

            if (mEvent)
            {
                delete mEventSpecifier;
                delete mEvent;
            }

            if (mSdClient)
                delete mSdClient;

            if (mNetworkLayer)
                delete mNetworkLayer;
        }
    }
}