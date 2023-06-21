#include <utility>
#include <iostream>
#include "./vehicle_id_message_handler.h"
#include "../include/doiplib/doip_controller.h"

void handleMessage(
    const DoipLib::DoipController &doipController,
    const DoipLib::EidVehicleIdRequest &request)
{
    std::vector<uint8_t> _serializedRequest;
    request.Serialize(_serializedRequest);

    std::vector<uint8_t> _serializedResponse;

    bool _isHandled{
        doipController.TryHandle(_serializedRequest, _serializedResponse)};

    if (_isHandled)
    {
        DoipLib::VehicleIdResponse _response;
        DoipLib::GenericNackType _nackCode;
        bool _isDeserialized{
            _response.TryDeserialize(_serializedResponse, _nackCode)};

        if (_isDeserialized)
        {
            std::cout << "VIN: " << _response.GetVin() << std::endl
                      << "LogicalAddress: " << _response.GetLogicalAddress() << std::endl;
        }
        else
        {
            std::cout << "Response is NOT deserialized correctly!\n";
        }
    }
    else
    {
        std::cout << "Request is NOT handled correctly!\n";
    }
}

int main()
{
    const uint16_t cLogicalAddress{0x0001};
    const std::size_t cIdSize{6};
    const std::array<uint8_t, cIdSize> cValidEid{
        0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    // Use EID as GID
    const std::array<uint8_t, cIdSize> cGid{cValidEid};
    const std::array<uint8_t, cIdSize> cInvalidEid{
        0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0};

    DoipLib::ControllerConfig _config;
    _config.protocolVersion = 0x02;
    _config.doipMaxRequestBytes = 64;
    _config.doIPInitialVehicleAnnouncementTime = std::chrono::seconds(0);
    _config.doIPVehicleAnnouncementCount = 3;
    _config.doIPVehicleAnnouncementInterval = std::chrono::seconds(1);

    DoipLib::VehicleIdMessageHandler _messageHandler(
        _config.protocolVersion,
        "ABCDEFGHIJKLMNOPQ",
        cLogicalAddress,
        cValidEid,
        cGid);

    DoipLib::DoipController _doipController(std::move(_config));
    _doipController.Register(
        DoipLib::PayloadType::VehicleIdRequestWithEid, &_messageHandler);

    DoipLib::EidVehicleIdRequest _validRequest(
        _config.protocolVersion, cValidEid);
    handleMessage(_doipController, _validRequest);

    DoipLib::EidVehicleIdRequest _invalidRequest(
        _config.protocolVersion, cInvalidEid);
    handleMessage(_doipController, _invalidRequest);

    return 0;
}