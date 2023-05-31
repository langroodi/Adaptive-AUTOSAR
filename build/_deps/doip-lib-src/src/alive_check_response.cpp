#include "doiplib/alive_check_response.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType AliveCheckResponse::cPayloadType;

    AliveCheckResponse::AliveCheckResponse() noexcept : Message(cPayloadType)
    {
    }

    AliveCheckResponse::AliveCheckResponse(
        uint8_t protocolVersion,
        uint16_t sourceAddress) noexcept : Message(protocolVersion, cPayloadType),
                                           mSourceAddress{sourceAddress}
    {
    }

    void AliveCheckResponse::GetPayload(std::vector<uint8_t> &payload) const
    {
        Convert::ToByteVector<uint16_t>(mSourceAddress, payload);
    }

    bool AliveCheckResponse::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const size_t cExpectedPayloadSize{sizeof(uint16_t)};

        if (payloadLength == cExpectedPayloadSize)
        {
            std::size_t _offset{cHeaderSize};
            mSourceAddress =
                Convert::ToUnsignedInteger<uint16_t>(payload, _offset);

            return true;
        }
        else
        {
            return false;
        }
    }

    uint16_t AliveCheckResponse::GetSourceAddress() const noexcept
    {
        return mSourceAddress;
    }
}