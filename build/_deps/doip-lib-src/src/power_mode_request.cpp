#include "doiplib/power_mode_request.h"

namespace DoipLib
{
    const PayloadType PowerModeRequest::cPayloadType;

    PowerModeRequest::PowerModeRequest() noexcept : Message(cPayloadType)
    {
    }


    PowerModeRequest::PowerModeRequest(
        uint8_t protocolVersion) noexcept : Message(protocolVersion, cPayloadType)
    {
    }

    void PowerModeRequest::GetPayload(std::vector<uint8_t> &payload) const
    {
        // The message contain no payload.
    }

    bool PowerModeRequest::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        // The message contain no payload.
        return payload.size() == cHeaderSize;
    }
}