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
        // The message contain no payload.
    }

    bool VehicleIdRequest::TrySetPayload(const std::vector<uint8_t> &payload)
    {
        // The message contain no payload.
        return payload.size() == cHeaderSize;
    }
}