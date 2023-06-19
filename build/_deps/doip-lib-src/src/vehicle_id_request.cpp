#include "doiplib/vehicle_id_request.h"

namespace DoipLib
{
    const PayloadType VehicleIdRequest::cPayloadType;

    VehicleIdRequest::VehicleIdRequest() noexcept : Message(cPayloadType)
    {
    }


    VehicleIdRequest::VehicleIdRequest(
        uint8_t protocolVersion) noexcept : Message(protocolVersion, cPayloadType)
    {
    }

    void VehicleIdRequest::GetPayload(std::vector<uint8_t> &payload) const
    {
        // The message contains empty payload.
        payload.clear();
    }

    bool VehicleIdRequest::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        // The message contains no payload.
        return payloadLength == 0;
    }
}