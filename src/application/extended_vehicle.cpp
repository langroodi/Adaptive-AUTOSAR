#include "../ara/com/someip/sd/sd_network_layer.h"
#include "./extended_vehicle.h"

namespace application
{
    const std::string ExtendedVehicle::cAppId{"ExtendedVehicle"};

    ExtendedVehicle::ExtendedVehicle() : ara::exec::helper::ModelledProcess(cAppId),
                                         mNetworkLayer{nullptr},
                                         mSdServer{nullptr}
    {
    }

    void ExtendedVehicle::configureNetworkLayer(const arxml::ArxmlReader &reader)
    {
        const std::string cNicIpAddress{"127.0.0.1"};

        const arxml::ArxmlNode cSdPortNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "PROVIDED-EVENT-GROUPS",
                                "SOMEIP-PROVIDED-EVENT-GROUP",
                                "EVENT-MULTICAST-UDP-PORT"})};

        const arxml::ArxmlNode cSdIpNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "PROVIDED-EVENT-GROUPS",
                                "SOMEIP-PROVIDED-EVENT-GROUP",
                                "IPV-4-MULTICAST-IP-ADDRESS"})};

        const auto cSdPort{cSdPortNode.GetValue<uint16_t>()};
        const auto cSdIp{cSdIpNode.GetValue<std::string>()};
        mNetworkLayer =
            new ara::com::someip::sd::SdNetworkLayer(
                &mPoller, cNicIpAddress, cSdIp, cSdPort);
    }

    helper::NetworkConfiguration ExtendedVehicle::getNetworkConfiguration(
        const arxml::ArxmlReader &reader)
    {
        const std::string cNetworkEndpoint{"ExtendedVehicleEP"};
        const std::string cApplicationEndpoint{"ServerUnicastTcp"};
        const ara::com::option::Layer4ProtocolType cProtocol{
            ara::com::option::Layer4ProtocolType::Tcp};

        helper::NetworkConfiguration _result;
        bool _successful{
            helper::TryGetNetworkConfiguration(
                reader, cNetworkEndpoint, cApplicationEndpoint, cProtocol,
                _result)};

        if (_successful)
        {
            return _result;
        }
        else
        {
            throw std::runtime_error("Fetching network configuration failed.");
        }
    }

    void ExtendedVehicle::configureSdServer(const arxml::ArxmlReader &reader)
    {
        const arxml::ArxmlNode cServiceIdNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "SERVICE-INTERFACE-DEPLOYMENT",
                                "SERVICE-INTERFACE-ID"})};

        const arxml::ArxmlNode cInstanceIdNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "SERVICE-INSTANCE-ID"})};

        const arxml::ArxmlNode cMajorVersionNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "SERVICE-INTERFACE-DEPLOYMENT",
                                "SERVICE-INTERFACE-VERSION",
                                "MAJOR-VERSION"})};

        const arxml::ArxmlNode cMinorVersionNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "SERVICE-INTERFACE-DEPLOYMENT",
                                "SERVICE-INTERFACE-VERSION",
                                "MINOR-VERSION"})};

        const arxml::ArxmlNode cInitialDelayMinNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "SD-SERVER-CONFIG",
                                "INITIAL-OFFER-BEHAVIOR",
                                "INITIAL-DELAY-MIN-VALUE"})};

        const arxml::ArxmlNode cInitialDelayMaxNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "PROVIDED-SOMEIP-SERVICE-INSTANCE",
                                "SD-SERVER-CONFIG",
                                "INITIAL-OFFER-BEHAVIOR",
                                "INITIAL-DELAY-MAX-VALUE"})};

        const auto cServiceId{cServiceIdNode.GetValue<uint16_t>()};
        const auto cInstanceId{cInstanceIdNode.GetValue<uint16_t>()};
        const auto cMajorVersion{cMajorVersionNode.GetValue<uint8_t>()};
        const auto cMinorVersion{cMinorVersionNode.GetValue<uint32_t>()};
        const auto cInitialDelayMin{cInitialDelayMinNode.GetValue<int>()};
        const auto cInitialDelayMax{cInitialDelayMaxNode.GetValue<int>()};

        helper::NetworkConfiguration _networkConfiguration{
            getNetworkConfiguration(reader)};
        
        mSdServer =
            new ara::com::someip::sd::SomeIpSdServer(
                mNetworkLayer,
                cServiceId,
                cInstanceId,
                cMajorVersion,
                cMinorVersion,
                _networkConfiguration.ipAddress,
                _networkConfiguration.portNumber,
                cInitialDelayMin,
                cInitialDelayMax);
    }

    int ExtendedVehicle::Main(
        const std::atomic_bool *cancellationToken,
        const std::map<std::string, std::string> &arguments)
    {
        const std::string cEvConfigArgument{"evconfig"};
        const std::string cEvConfigFilepath{arguments.at(cEvConfigArgument)};
        const arxml::ArxmlReader cReader(cEvConfigFilepath);

        ara::log::LogStream _logStream;

        try
        {
            configureNetworkLayer(cReader);
            configureSdServer(cReader);
            
            _logStream << "Extended Vehicle AA has been initialized.";
            Log(cLogLevel, _logStream);

            bool _running{true};

            while (!cancellationToken->load() && _running)
            {
                _running = WaitForActivation();
                mPoller.TryPoll();
            }

            _logStream.Flush();
            _logStream << "Extended Vehicle AA has been terminated.";
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

    ExtendedVehicle::~ExtendedVehicle()
    {
        if (mSdServer)
            delete mSdServer;

        if (mNetworkLayer)
            delete mNetworkLayer;
    }
}