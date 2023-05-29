#include <json/json.h>
#include "../ara/com/someip/sd/sd_network_layer.h"
#include "../ara/diag/conversation.h"
#include "../application/helper/argument_configuration.h"
#include "./extended_vehicle.h"

namespace application
{
    const std::string ExtendedVehicle::cAppId{"ExtendedVehicle"};
    const ara::core::InstanceSpecifier ExtendedVehicle::cSeInstance{"ExtendedVehicleSE"};

    ExtendedVehicle::ExtendedVehicle(
        AsyncBsdSocketLib::Poller *poller,
        ara::phm::CheckpointCommunicator *checkpointCommunicator) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                                                    mSupervisedEntity{cSeInstance, checkpointCommunicator},
                                                                    mNetworkLayer{nullptr},
                                                                    mSdServer{nullptr},
                                                                    mCurl{nullptr}
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
                Poller, cNicIpAddress, cSdIp, cSdPort);
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

    bool ExtendedVehicle::tryConfigureRestCommunication(
        std::string apiKey, std::string bearerToken, std::string &vin)
    {
        const bool cCollectJsonComments{false};
        const std::string cVehiclesKey{"vehicles"};
        const std::string cErrorKey{"error"};
        const std::string cRequestUrl{
            "https://api.volvocars.com/extended-vehicle/v1/vehicles"};

        mCurl = new helper::CurlWrapper(apiKey, bearerToken);

        std::string _response;
        bool _successful{mCurl->TryExecute(cRequestUrl, &_response)};

        if (!_successful)
        {
            return false;
        }

        Json::Value _jsonResponse;
        Json::Reader _jsonReader;

        _successful =
            _jsonReader.parse(_response, _jsonResponse, cCollectJsonComments);

        if (!_successful)
        {
            return false;
        }

        ara::log::LogStream _logStream;

        if (_jsonResponse.isMember(cVehiclesKey))
        {
            vin = _jsonResponse[cVehiclesKey][0]["id"].asString();
            mResourcesUrl = cRequestUrl + "/" + vin + "/resources";
            _logStream << "The VIN is set to " << vin;
            Log(cLogLevel, _logStream);

            return true;
        }
        else if (_jsonResponse.isMember(cErrorKey))
        {
            std::string _message = _jsonResponse[cErrorKey]["message"].asString();
            _logStream << "Setting the VIN failed. " << _message;
            Log(cErrorLevel, _logStream);

            return false;
        }
        else
        {
            _logStream << "Setting the VIN failed due to unexpected RESTful response format.";
            Log(cErrorLevel, _logStream);

            return false;
        }
    }

    DoipLib::ControllerConfig ExtendedVehicle::getDoipConfiguration(
        const arxml::ArxmlReader &reader)
    {
        const arxml::ArxmlNode cAnnouncementTimeNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "DO-IP-INSTANTIATION",
                                "NETWORK-INTERFACES",
                                "DO-IP-NETWORK-CONFIGURATION",
                                "MAX-INITIAL-VEHICLE-ANNOUNCEMENT-TIME"})};

        const arxml::ArxmlNode cAnnouncementCountNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "DO-IP-INSTANTIATION",
                                "NETWORK-INTERFACES",
                                "DO-IP-NETWORK-CONFIGURATION",
                                "VEHICLE-ANNOUNCEMENT-COUNT"})};

        const arxml::ArxmlNode cAnnouncementIntervalNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "DO-IP-INSTANTIATION",
                                "NETWORK-INTERFACES",
                                "DO-IP-NETWORK-CONFIGURATION",
                                "VEHICLE-ANNOUNCEMENT-INTERVAL"})};

        DoipLib::ControllerConfig _result;

        const auto cAnnouncementTime{cAnnouncementTimeNode.GetValue<int>()};
        _result.doIPInitialVehicleAnnouncementTime =
            std::chrono::seconds(cAnnouncementTime);

        const auto cAnnouncementCount{cAnnouncementCountNode.GetValue<uint8_t>()};
        _result.doIPVehicleAnnouncementCount = cAnnouncementCount;

        const auto cAnnouncementInterval{cAnnouncementIntervalNode.GetValue<int>()};
        _result.doIPVehicleAnnouncementInterval =
            std::chrono::seconds(cAnnouncementInterval);

        const uint8_t cProtocolVersion{2};
        _result.protocolVersion = cProtocolVersion;

        _result.doipMaxRequestBytes = doip::DoipServer::cDoipPacketSize;

        return _result;
    }

    void ExtendedVehicle::configureDoipServer(
        const arxml::ArxmlReader &reader, std::string &&vin)
    {
        const arxml::ArxmlNode cLogicalAddressNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "DO-IP-INSTANTIATION",
                                "LOGICAL-ADDRESS"})};

        const arxml::ArxmlNode cEidNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "DO-IP-INSTANTIATION",
                                "EID"})};

        const arxml::ArxmlNode cGidNode{
            reader.GetRootNode({"AUTOSAR",
                                "AR-PACKAGES",
                                "AR-PACKAGE",
                                "ELEMENTS",
                                "DO-IP-INSTANTIATION",
                                "GID"})};

        const auto cLogicalAddress{cLogicalAddressNode.GetValue<uint16_t>()};
        const auto cEid{cEidNode.GetValue<uint64_t>()};
        const auto cGid{cGidNode.GetValue<uint64_t>()};

        helper::NetworkConfiguration _networkConfiguration{
            getNetworkConfiguration(reader)};

        DoipLib::ControllerConfig _controllConfig{getDoipConfiguration(reader)};

        mDoipServer =
            new doip::DoipServer(
                Poller,
                mCurl,
                mResourcesUrl,
                _networkConfiguration.ipAddress,
                _networkConfiguration.portNumber,
                std::move(_controllConfig),
                std::move(vin),
                cLogicalAddress,
                cEid,
                cGid);
    }

    int ExtendedVehicle::Main(
        const std::atomic_bool *cancellationToken,
        const std::map<std::string, std::string> &arguments)
    {
        const std::string cEvConfigArgument{helper::ArgumentConfiguration::cEvConfigArgument};
        const std::string cEvConfigFilepath{arguments.at(cEvConfigArgument)};
        const arxml::ArxmlReader cReader(cEvConfigFilepath);

        ara::log::LogStream _logStream;

        try
        {
            bool _running{true};

            configureNetworkLayer(cReader);
            configureSdServer(cReader);

            _logStream << "Extended Vehicle AA has been initialized.";
            Log(cLogLevel, _logStream);

            std::string _vin;
            bool cConfigured{tryConfigureRestCommunication(
                arguments.at(helper::ArgumentConfiguration::cApiKeyArgument),
                arguments.at(helper::ArgumentConfiguration::cBearerTokenArgument),
                _vin)};

            if (cConfigured)
            {
                configureDoipServer(cReader, std::move(_vin));
                mSdServer->Start();
            }

            while (!cancellationToken->load() && _running)
            {
                mSupervisedEntity.ReportCheckpoint(
                    PhmCheckpointType::AliveCheckpoint);
                mSupervisedEntity.ReportCheckpoint(
                    PhmCheckpointType::DeadlineSourceCheckpoint);

                _running = WaitForActivation();

                mSupervisedEntity.ReportCheckpoint(
                    PhmCheckpointType::DeadlineTargetCheckpoint);
            }

            _logStream.Flush();
            if (ara::diag::Conversation::GetCurrentActiveConversations().size() == 0)
            {
                _logStream << "There was no active diagnostic conversation at the termination.";
            }
            else
            {
                _logStream << "There were still some active diagnostic conversations at the termination.";
            }

            Log(cLogLevel, _logStream);

            delete mSdServer;
            mSdServer = nullptr;

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
        if (mDoipServer)
            delete mDoipServer;

        if (mCurl)
            delete mCurl;

        if (mSdServer)
            delete mSdServer;

        if (mNetworkLayer)
            delete mNetworkLayer;
    }
}