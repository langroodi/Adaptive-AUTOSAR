#include "doiplib/routing_activation_request.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType RoutingActivationRequest::cPayloadType;

    RoutingActivationRequest::RoutingActivationRequest() noexcept : Message(cPayloadType)
    {
    }

    RoutingActivationRequest::RoutingActivationRequest(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint8_t activationType,
        bool hasOemSpecificData,
        uint32_t oemSpecificData) noexcept : Message(protocolVersion, cPayloadType),
                                             mSourceAddress{sourceAddress},
                                             mActivationType{activationType},
                                             mHasOemSpecificData{hasOemSpecificData},
                                             mOemSpecificData{oemSpecificData}
    {
    }

    RoutingActivationRequest::RoutingActivationRequest(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint8_t activationType) noexcept : RoutingActivationRequest(protocolVersion, sourceAddress, activationType, false)
    {
    }

    RoutingActivationRequest::RoutingActivationRequest(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint8_t activationType,
        uint32_t oemSpecificData) noexcept : RoutingActivationRequest(protocolVersion, sourceAddress, activationType, true, oemSpecificData)
    {
    }

    void RoutingActivationRequest::GetPayload(std::vector<uint8_t> &payload) const
    {
        if (mHasOemSpecificData)
        {
            Convert::ToByteVector<uint32_t>(mOemSpecificData, payload);
        }

        Convert::ToByteVector<uint32_t>(cIsoReserved, payload);
        payload.insert(payload.begin(), mActivationType);
        Convert::ToByteVector<uint16_t>(mSourceAddress, payload);
    }

    bool RoutingActivationRequest::TrySetCompulsoryPayload(
        const std::vector<uint8_t> &payload)
    {
        std::size_t _offset{cHeaderSize};

        auto _sourceAddress{
            Convert::ToUnsignedInteger<uint16_t>(payload, _offset)};
        auto _activationType{
            Convert::ToUnsignedInteger<uint8_t>(payload, _offset)};

        // Reserved bytes validation
        auto _actualReservedInt{
            Convert::ToUnsignedInteger<uint32_t>(payload, _offset)};

        if (_actualReservedInt == cIsoReserved)
        {
            mSourceAddress = _sourceAddress;
            mActivationType = _activationType;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool RoutingActivationRequest::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cExpectedPayloadSizeMin{7};
        const std::size_t cExpectedPayloadSizeMax{11};

        if (payloadLength == cExpectedPayloadSizeMin)
        {
            bool _succeed{TrySetCompulsoryPayload(payload)};
            if (_succeed)
            {
                // No OEM-specific data
                mHasOemSpecificData = false;
            }

            return _succeed;
        }
        else if (payloadLength == cExpectedPayloadSizeMax)
        {
            bool _succeed{TrySetCompulsoryPayload(payload)};
            if (_succeed)
            {
                // Has OEM-speific data
                mHasOemSpecificData = true;
                std::size_t _offset{
                    static_cast<std::size_t>(
                        cHeaderSize + cExpectedPayloadSizeMin)};
                mOemSpecificData =
                    Convert::ToUnsignedInteger<uint32_t>(payload, _offset);
            }

            return _succeed;
        }
        else
        {
            return false;
        }
    }

    uint16_t RoutingActivationRequest::GetSourceAddress() const noexcept
    {
        return mSourceAddress;
    }

    uint8_t RoutingActivationRequest::GetActivationType() const noexcept
    {
        return mActivationType;
    }

    bool RoutingActivationRequest::TryGetOemSpecificData(uint32_t &oemSpecificData) const noexcept
    {
        if (mHasOemSpecificData)
        {
            oemSpecificData = mOemSpecificData;
            return true;
        }
        else
        {
            return false;
        }
    }
}
