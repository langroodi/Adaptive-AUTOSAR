#include "doiplib/generic_nack.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType GenericNack::cPayloadType;

    GenericNack::GenericNack() noexcept : Message(cPayloadType)
    {
    }

    GenericNack::GenericNack(
        uint8_t protocolVersion,
        GenericNackType nackCode) noexcept : Message(protocolVersion, cPayloadType),
                                             mNackCode{nackCode}
    {
    }

    void GenericNack::GetPayload(std::vector<uint8_t> &payload) const
    {
        auto _nackCodeByte{static_cast<uint8_t>(mNackCode)};
        payload.insert(payload.begin(), _nackCodeByte);
    }

    bool GenericNack::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cExpectedPayloadSize{sizeof(GenericNackType)};

        if (payloadLength == cExpectedPayloadSize)
        {
            std::size_t _offset{cHeaderSize};
            mNackCode = Convert::ToEnum<GenericNackType>(payload, _offset);
            return true;
        }
        else
        {
            return false;
        }
    }

    GenericNackType GenericNack::GetNackCode() const noexcept
    {
        return mNackCode;
    }
}