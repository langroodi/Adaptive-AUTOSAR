#include "doiplib/alive_check_request.h"

namespace DoipLib
{
    const PayloadType AliveCheckRequest::cPayloadType;

    AliveCheckRequest::AliveCheckRequest() noexcept : Message(cPayloadType)
    {
    }


    AliveCheckRequest::AliveCheckRequest(
        uint8_t protocolVersion) noexcept : Message(protocolVersion, cPayloadType)
    {
    }

    void AliveCheckRequest::GetPayload(std::vector<uint8_t> &payload) const
    {
        // The message contains empty payload.
        payload.clear();
    }

    bool AliveCheckRequest::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        // The message contains no payload.
        return payloadLength == 0;
    }
}