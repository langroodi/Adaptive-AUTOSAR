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

    bool DiagMessageNack::TrySetPayload(const std::vector<uint8_t> &payload)
    {
        const std::size_t cExpectedSizeMin{
            static_cast<std::size_t>(cHeaderSize + 5)};

        if (payload.size() == cExpectedSizeMin)
        {
            SetCompulsoryPayload(payload);
            // No previous message payload
            mHasPreviousMessage = false;

            return true;
        }
        else if (payload.size() > cExpectedSizeMin)
        {
            SetCompulsoryPayload(payload);
            // Has revious message payload
            mHasPreviousMessage = true;
            auto _beginItr{payload.cbegin() + cExpectedSizeMin};
            mPreviousMessage = std::vector<uint8_t>(_beginItr, payload.cend());

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