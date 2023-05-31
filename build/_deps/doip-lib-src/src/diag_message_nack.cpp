#include <utility>
#include "doiplib/diag_message_nack.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType DiagMessageNack::cPayloadType;

    DiagMessageNack::DiagMessageNack() : Message(cPayloadType)
    {
    }

    DiagMessageNack::DiagMessageNack(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        DiagNackType nackCode) : Message(protocolVersion, cPayloadType),
                                 mSourceAddress{sourceAddress},
                                 mTargetAddress{targetAddress},
                                 mNackCode{nackCode},
                                 mHasPreviousMessage{false}
    {
    }

    DiagMessageNack::DiagMessageNack(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        DiagNackType nackCode,
        const std::vector<uint8_t> &previousMessage) : Message(protocolVersion, cPayloadType),
                                                       mSourceAddress{sourceAddress},
                                                       mTargetAddress{targetAddress},
                                                       mHasPreviousMessage{true},
                                                       mNackCode{nackCode},
                                                       mPreviousMessage{previousMessage}
    {
    }

    DiagMessageNack::DiagMessageNack(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        DiagNackType nackCode,
        std::vector<uint8_t> &&previousMessage) : Message(protocolVersion, cPayloadType),
                                                  mSourceAddress{sourceAddress},
                                                  mTargetAddress{targetAddress},
                                                  mNackCode{nackCode},
                                                  mHasPreviousMessage{true},
                                                  mPreviousMessage{std::move(previousMessage)}
    {
    }

    void DiagMessageNack::GetPayload(std::vector<uint8_t> &payload) const
    {
        if (mHasPreviousMessage)
        {
            payload.insert(
                payload.begin(), mPreviousMessage.cbegin(), mPreviousMessage.cend());
        }

        auto _nackCodeByte{static_cast<uint8_t>(mNackCode)};
        payload.insert(payload.begin(), _nackCodeByte);

        Convert::ToByteVector<uint16_t>(mTargetAddress, payload);

        Convert::ToByteVector<uint16_t>(mSourceAddress, payload);
    }

    void DiagMessageNack::SetCompulsoryPayload(const std::vector<uint8_t> &payload)
    {
        std::size_t _offset{cHeaderSize};

        mSourceAddress = Convert::ToUnsignedInteger<uint16_t>(payload, _offset);
        mTargetAddress = Convert::ToUnsignedInteger<uint16_t>(payload, _offset);
        mNackCode = Convert::ToEnum<DiagNackType>(payload, _offset);
    }

    bool DiagMessageNack::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cCompulsoryPayloadSize{5};
        const std::size_t cExpectedSizeMin{
            static_cast<std::size_t>(cHeaderSize + cCompulsoryPayloadSize)};

        if (payload.size() == cExpectedSizeMin)
        {
            SetCompulsoryPayload(payload);
            // No previous message payload
            mHasPreviousMessage = false;

            return true;
        }
        else if (payload.size() > cExpectedSizeMin)
        {
            const auto cPayloadSize{static_cast<std::size_t>(payloadLength)};
            
            SetCompulsoryPayload(payload);
            // Has revious message payload
            mHasPreviousMessage = true;
            const auto cBeginItr{payload.cbegin() + cExpectedSizeMin};
            // [Header: 8 bytes]<cBeginItr>[CompulsoryPayload: 5 bytes][PreviousMessage: var bytes]
            const auto cEndItr{
                cBeginItr + (cPayloadSize - cCompulsoryPayloadSize)};
            mPreviousMessage = std::vector<uint8_t>(cBeginItr, cEndItr);

            return true;
        }
        else
        {
            return false;
        }
    }

    uint16_t DiagMessageNack::GetSourceAddress() const noexcept
    {
        return mSourceAddress;
    }

    uint16_t DiagMessageNack::GetTargetAddress() const noexcept
    {
        return mTargetAddress;
    }

    DiagNackType DiagMessageNack::GetNackCode() const noexcept
    {
        return mNackCode;
    }

    bool DiagMessageNack::TryGetPreviousMessage(std::vector<uint8_t> &previousMessage)
    {
        if (mHasPreviousMessage)
        {
            previousMessage = mPreviousMessage;
        }

        return mHasPreviousMessage;
    }
}