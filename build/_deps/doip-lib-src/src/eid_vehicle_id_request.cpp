#include "doiplib/eid_vehicle_id_request.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType EidVehicleIdRequest::cPayloadType;
    constexpr std::size_t EidVehicleIdRequest::cEidSize;

    EidVehicleIdRequest::EidVehicleIdRequest() noexcept : Message(cPayloadType)
    {
    }

    EidVehicleIdRequest::EidVehicleIdRequest(
        uint8_t protocolVersion,
        std::array<uint8_t, cEidSize> eid) : Message(protocolVersion, cPayloadType),
                                             mEid{eid}
    {
    }

    void EidVehicleIdRequest::GetPayload(std::vector<uint8_t> &payload) const
    {
        payload = std::vector<uint8_t>(mEid.cbegin(), mEid.cend());
    }

    bool EidVehicleIdRequest::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cExpectedSize{
            static_cast<std::size_t>(cHeaderSize + cEidSize)};

        if (payload.size() == cExpectedSize)
        {
            std::size_t _offset{cHeaderSize};
            mEid = Convert::ToByteArray<cEidSize>(payload, _offset);

            return true;
        }
        else
        {
            return false;
        }
    }

    std::array<uint8_t, EidVehicleIdRequest::cEidSize> EidVehicleIdRequest::GetEid() const
    {
        return mEid;
    }
}