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
        // The message contains empty payload.
        payload.clear();
    }

    bool EntityStatusRequest::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        // The message contains no payload.
        return payloadLength == 0;
    }
}