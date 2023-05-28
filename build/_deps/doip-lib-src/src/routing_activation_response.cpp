#include "doiplib/routing_activation_response.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType RoutingActivationResponse::cPayloadType;

    RoutingActivationResponse::RoutingActivationResponse() noexcept : Message(cPayloadType)
    {
    }

    RoutingActivationResponse::RoutingActivationResponse(
        uint8_t protocolVersion,
        uint16_t testerLogicalAddress,
        uint16_t entityLogicalAddress,
        RoutingActivationResponseType responseCode,
        bool hasOemSpecificData,
        uint32_t oemSpecificData) noexcept : Message(protocolVersion, cPayloadType),
                                             mTesterLogicalAddress{testerLogicalAddress},
                                             mEntityLogicalAddress{entityLogicalAddress},
                                             mResponseCode{responseCode},
                                             mHasOemSpecificData{hasOemSpecificData},
                                             mOemSpecificData{oemSpecificData}
    {
    }

    RoutingActivationResponse::RoutingActivationResponse(
        uint8_t protocolVersion,
        uint16_t testerLogicalAddress,
        uint16_t entityLogicalAddress,
        RoutingActivationResponseType responseCode) noexcept : RoutingActivationResponse(protocolVersion, testerLogicalAddress, entityLogicalAddress, responseCode, false)
    {
    }

    RoutingActivationResponse::RoutingActivationResponse(
        uint8_t protocolVersion,
        uint16_t testerLogicalAddress,
        uint16_t entityLogicalAddress,
        RoutingActivationResponseType responseCode,
        uint32_t oemSpecificData) noexcept : RoutingActivationResponse(protocolVersion, testerLogicalAddress, entityLogicalAddress, responseCode, true, oemSpecificData)
    {
    }

    void RoutingActivationResponse::GetPayload(std::vector<uint8_t> &payload) const
    {
        if (mHasOemSpecificData)
        {
            Convert::ToByteVector<uint32_t>(mOemSpecificData, payload);
        }

        Convert::ToByteVector<uint32_t>(cIsoReserved, payload);

        auto _responseCodeByte{static_cast<uint8_t>(mResponseCode)};
        payload.insert(payload.begin(), _responseCodeByte);

        Convert::ToByteVector<uint16_t>(mEntityLogicalAddress, payload);

        Convert::ToByteVector<uint16_t>(mTesterLogicalAddress, payload);
    }

    void RoutingActivationResponse::SetPayload(const std::vector<uint8_t> &payload)
    {
        std::size_t _offset{cHeaderSize};

        mTesterLogicalAddress = Convert::ToUnsignedInteger<uint16_t>(payload, _offset);
        mEntityLogicalAddress = Convert::ToUnsignedInteger<uint16_t>(payload, _offset);
        mResponseCode = Convert::ToEnum<RoutingActivationResponseType>(payload, _offset);
    }

    bool RoutingActivationResponse::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cExpectedPayloadSizeMin{9};
        const std::size_t cExpectedPayloadSizeMax{13};

        if (payloadLength == cExpectedPayloadSizeMin)
        {
            SetPayload(payload);
            // No OEM-specific data
            mHasOemSpecificData = false;

            return true;
        }
        else if (payloadLength == cExpectedPayloadSizeMax)
        {
            SetPayload(payload);
            // Has OEM-speific data
            mHasOemSpecificData = true;
            std::size_t _offset{
                static_cast<std::size_t>(cHeaderSize + cExpectedPayloadSizeMin)};
            mOemSpecificData =
                Convert::ToUnsignedInteger<uint32_t>(payload, _offset);

            return true;
        }
        else
        {
            return false;
        }
    }

    uint16_t RoutingActivationResponse::GetTesterLogicalAddress() const noexcept
    {
        return mTesterLogicalAddress;
    }

    uint16_t RoutingActivationResponse::GetEntityLogicalAddress() const noexcept
    {
        return mEntityLogicalAddress;
    }

    RoutingActivationResponseType RoutingActivationResponse::GetResponseCode() const noexcept
    {
        return mResponseCode;
    }

    bool RoutingActivationResponse::TryGetOemSpecificData(uint32_t &oemSpecificData) const noexcept
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
