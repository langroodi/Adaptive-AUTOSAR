#include "../../ara/com/someip/sd/sd_network_layer.h"
#include "../../application/helper/argument_configuration.h"
#include "./diagnostic_manager.h"

namespace application
{
    namespace platform
    {
        const std::string DiagnosticManager::cAppId{"DiagnosticManager"};
        const std::string DiagnosticManager::cSerialPort{"/dev/ttyUSB0"};
        const speed_t DiagnosticManager::cBaudrate{115200};
        const bool DiagnosticManager::cSupportExtended{false};
        const ObdEmulator::CanBusSpeed DiagnosticManager::cSpeed{ObdEmulator::CanBusSpeed::Speed250kbps};

        DiagnosticManager::DiagnosticManager(AsyncBsdSocketLib::Poller *poller) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                                                                  mNetworkLayer{nullptr},
                                                                                  mSdClient{nullptr},
                                                                                  mEvent{nullptr},
                                                                                  mMonitor{nullptr},
                                                                                  mSerialCommunication(cSerialPort, cBaudrate),
                                                                                  mCanDriver(cSpeed, cSupportExtended),
                                                                                  mObdToDoipConverter{nullptr},
                                                                                  mObdEmulator{nullptr}
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

        void DiagnosticManager::onEventStatusChanged(
            ara::diag::EventStatusByte eventStatus)
        {
            const auto cDebouncingStatusResult{mEvent->GetDebouncingStatus()};

            if (!cDebouncingStatusResult.HasValue())
            {
                return;
            }

            const ara::diag::DebouncingState cDebouncingStatus{
                cDebouncingStatusResult.Value()};

            if (cDebouncingStatus ==
                ara::diag::DebouncingState::kFinallyHealed)
            {
                ara::log::LogStream _logStream;
                _logStream << "Telematic Control Module (Extended Vehicle AA) is discovered.";
                Log(cLogLevel, _logStream);

                std::string _ipAddress;
                uint16_t _port;
                bool _successful{
                    mSdClient->TryGetOfferedEndpoint(_ipAddress, _port)};
                if (!_successful)
                {
                    throw std::runtime_error("Fetching offered endpoint failed.");
                }

                mObdToDoipConverter =
                    new doip::ObdToDoipConverter(Poller, _ipAddress, _port);

                mObdEmulator =
                    new ObdEmulator::ObdEmulator(
                        &mSerialCommunication,
                        &mCanDriver,
                        {mObdToDoipConverter});

                _successful = mObdEmulator->TryStartAsync();
                if (!_successful)
                {
                    throw std::runtime_error("Starting OBD-II emulator failed.");
                }
            }
            else if (cDebouncingStatus ==
                     ara::diag::DebouncingState::kFinallyDefective)
            {
                const ara::diag::DTCFormatType cDtcFormat{
                    ara::diag::DTCFormatType::kDTCFormatUDS};
                const auto cDtcNumberResult{mEvent->GetDTCNumber(cDtcFormat)};

                ara::log::LogStream _logStream;
                _logStream << mEventSpecifier->ToString()
                           << " is failed with DTC "
                           << cDtcNumberResult.Value();
                Log(cErrorLevel, _logStream);
            }
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

            auto _onEventStatusChanged{
                std::bind(
                    &DiagnosticManager::onEventStatusChanged,
                    this,
                    std::placeholders::_1)};
            mEvent->SetEventStatusChangedNotifier(_onEventStatusChanged);
        }

        void DiagnosticManager::onInitMonitor(ara::diag::InitMonitorReason reason)
        {
            ara::log::LogStream _logStream;

            switch (reason)
            {
            case ara::diag::InitMonitorReason::kReenabled:
                _logStream << mMonitorSpecifier->ToString() << " is offered.";

                break;

            case ara::diag::InitMonitorReason::kDisabled:
                _logStream << mMonitorSpecifier->ToString() << " offer is stopped.";
                break;

            default:
                auto _reasonInt{static_cast<uint32_t>(reason)};
                _logStream << mMonitorSpecifier->ToString() << "'s reason is " << _reasonInt;
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
            mMonitor->AttachEvent(mEvent);
            mMonitor->Offer();
        }

        void DiagnosticManager::checkServiceDiscovery()
        {
            const ara::com::helper::SdClientState cState{
                mSdClient->GetState()};

            if (cState == ara::com::helper::SdClientState::ServiceReady ||
                cState == ara::com::helper::SdClientState::ServiceSeen)
            {
                mMonitor->ReportMonitorAction(
                    ara::diag::MonitorAction::kPrepassed);
            }
            else if (cState == ara::com::helper::SdClientState::Stopped ||
                     cState == ara::com::helper::SdClientState::ServiceNotSeen)
            {
                mMonitor->ReportMonitorAction(
                    ara::diag::MonitorAction::kPrefailed);
            }
        }

        int DiagnosticManager::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const std::string cDmConfigArgument{helper::ArgumentConfiguration::cDmConfigArgument};
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
                    checkServiceDiscovery();
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
            if (mObdEmulator)
            {
                delete mObdEmulator;
            }

            if (mObdToDoipConverter)
            {
                delete mObdToDoipConverter;
            }

            if (mMonitor)
            {
                mMonitor->StopOffer();
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