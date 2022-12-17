#include "doiplib/entity_status_request.h"

namespace DoipLib
{
    const PayloadType EntityStatusRequest::cPayloadType;

    EntityStatusRequest::EntityStatusRequest() noexcept : Message(cPayloadType)
    {
    }


    EntityStatusRequest::EntityStatusRequest(
        uint8_t protocolVersion) noexcept : Message(protocolVersion, cPayloadType)
    {
    }

    void EntityStatusRequest::GetPayload(std::vector<uint8_t> &payload) const
    {
        // The message contain no payload.
    }

    bool EntityStatusRequest::TrySetPayload(const std::vector<uint8_t> &payload)
    {
        // The message contain no payload.
        return payload.size() == cHeaderSize;
    }
}