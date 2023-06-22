#include "doiplib/power_mode_response.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType PowerModeResponse::cPayloadType;

    PowerModeResponse::PowerModeResponse() noexcept : Message(cPayloadType)
    {
    }

    PowerModeResponse::PowerModeResponse(
        uint8_t protocolVersion,
        PowerModeType powerMode) noexcept : Message(protocolVersion, cPayloadType),
                                            mPowerMode{powerMode}
    {
    }

    void PowerModeResponse::GetPayload(std::vector<uint8_t> &payload) const
    {
        auto _powerModeByte{static_cast<uint8_t>(mPowerMode)};
        payload.insert(payload.begin(), _powerModeByte);
    }

    bool PowerModeResponse::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cExpectedPayloadSize{sizeof(PowerModeType)};

        if (payloadLength == cExpectedPayloadSize)
        {
            std::size_t _offset{cHeaderSize};
            mPowerMode = Convert::ToEnum<PowerModeType>(payload, _offset);
            return true;
        }
        else
        {
            return false;
        }
    }

    PowerModeType PowerModeResponse::GetPowerMode() const noexcept
    {
        return mPowerMode;
    }
}