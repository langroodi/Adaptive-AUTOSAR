#include <utility>
#include "./vehicle_id_message_handler.h"

namespace DoipLib
{
    constexpr std::size_t VehicleIdMessageHandler::cIdSize;
    const uint8_t VehicleIdMessageHandler::cFurtherAction;

    VehicleIdMessageHandler::VehicleIdMessageHandler(
        uint8_t protocolVersion,
        std::string &&vin,
        uint16_t logicalAddress,
        std::array<uint8_t, cIdSize> eid,
        std::array<uint8_t, cIdSize> gid) : mResponse{protocolVersion, std::move(vin), logicalAddress, eid, gid, cFurtherAction}
    {
    }

    Message *VehicleIdMessageHandler::GetMessage()
    {
        return static_cast<Message *>(&mRequest);
    }

    bool VehicleIdMessageHandler::TryHandle(
        const Message *request, std::vector<uint8_t> &response) const
    {
        auto _eidVehicleIdRequest{
            dynamic_cast<const EidVehicleIdRequest *>(request)};

        // Validate the received EID within the request
        if (_eidVehicleIdRequest->GetEid() == mResponse.GetEid())
        {
            mResponse.Serialize(response);
            return true;
        }
        else
        {
            return false;
        }
    }
}