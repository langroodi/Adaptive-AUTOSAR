#include <utility>
#include "doiplib/diag_message_ack.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType DiagMessageAck::cPayloadType;

    DiagMessageAck::DiagMessageAck() : Message(cPayloadType)
    {
    }

    DiagMessageAck::DiagMessageAck(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress) : Message(protocolVersion, cPayloadType),
                                  mSourceAddress{sourceAddress},
                                  mTargetAddress{targetAddress},
                                  mHasPreviousMessage{false}
    {
    }

    DiagMessageAck::DiagMessageAck(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        const std::vector<uint8_t> &previousMessage) : Message(protocolVersion, cPayloadType),
                                                       mSourceAddress{sourceAddress},
                                                       mTargetAddress{targetAddress},
                                                       mHasPreviousMessage{true},
                                                       mPreviousMessage{previousMessage}
    {
    }

    DiagMessageAck::DiagMessageAck(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        std::vector<uint8_t> &&previousMessage) : Message(protocolVersion, cPayloadType),
                                                  mSourceAddress{sourceAddress},
                                                  mTargetAddress{targetAddress},
                                                  mHasPreviousMessage{true},
                                                  mPreviousMessage{std::move(previousMessage)}
    {
    }

    void DiagMessageAck::GetPayload(std::vector<uint8_t> &payload) const
    {
        if (mHasPreviousMessage)
        {
            payload.insert(
                payload.begin(), mPreviousMessage.cbegin(), mPreviousMessage.cend());
        }

        Convert::ToByteVector<uint8_t>(cAckCode, payload);
        Convert::ToByteVector<uint16_t>(mTargetAddress, payload);
        Convert::ToByteVector<uint16_t>(mSourceAddress, payload);
    }

    bool DiagMessageAck::TrySetCompulsoryPayload(
        const std::vector<uint8_t> &payload)
    {
        std::size_t _offset{cHeaderSize};

        auto _sourceAddress{
            Convert::ToUnsignedInteger<uint16_t>(payload, _offset)};
        auto _targetAddress{
            Convert::ToUnsignedInteger<uint16_t>(payload, _offset)};

        // Reserved bytes validation
        uint8_t _actualAckCode{payload.at(_offset)};

        if (_actualAckCode == cAckCode)
        {
            mSourceAddress = _sourceAddress;
            mTargetAddress = _targetAddress;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool DiagMessageAck::TrySetPayload(const std::vector<uint8_t> &payload)
    {
        const std::size_t cExpectedSizeMin{
            static_cast<std::size_t>(cHeaderSize + 5)};

        if (payload.size() == cExpectedSizeMin)
        {
            bool _succeed{TrySetCompulsoryPayload(payload)};
            if (_succeed)
            {
                // No previous message payload
                mHasPreviousMessage = false;
            }

            return _succeed;
        }
        else if (payload.size() > cExpectedSizeMin)
        {
            bool _succeed{TrySetCompulsoryPayload(payload)};
            if (_succeed)
            {
                // Has revious message payload
                mHasPreviousMessage = true;
                auto _beginItr{payload.cbegin() + cExpectedSizeMin};
                mPreviousMessage = std::vector<uint8_t>(_beginItr, payload.cend());
            }

            return _succeed;
        }
        else
        {
            return false;
        }
    }

    uint16_t DiagMessageAck::GetSourceAddress() const noexcept
    {
        return mSourceAddress;
    }

    uint16_t DiagMessageAck::GetTargetAddress() const noexcept
    {
        return mTargetAddress;
    }

    bool DiagMessageAck::TryGetPreviousMessage(std::vector<uint8_t> &previousMessage)
    {
        if (mHasPreviousMessage)
        {
            previousMessage = mPreviousMessage;
        }

        return mHasPreviousMessage;
    }
}