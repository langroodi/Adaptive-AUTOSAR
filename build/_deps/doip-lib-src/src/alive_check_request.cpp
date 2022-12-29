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
        // The message contain no payload.
    }

    bool AliveCheckRequest::TrySetPayload(const std::vector<uint8_t> &payload)
    {
        // The message contain no payload.
        return payload.size() == cHeaderSize;
    }
}