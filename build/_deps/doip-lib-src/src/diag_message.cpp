#include <utility>
#include "doiplib/diag_message.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType DiagMessage::cPayloadType;

    DiagMessage::DiagMessage() : Message(cPayloadType)
    {
    }

    DiagMessage::DiagMessage(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        const std::vector<uint8_t> &userData) : Message(protocolVersion, cPayloadType),
                                                mSourceAddress{sourceAddress},
                                                mTargetAddress{targetAddress},
                                                mUserData{userData}
    {
    }

    DiagMessage::DiagMessage(
        uint8_t protocolVersion,
        uint16_t sourceAddress,
        uint16_t targetAddress,
        std::vector<uint8_t> &&userData) : Message(protocolVersion, cPayloadType),
                                           mSourceAddress{sourceAddress},
                                           mTargetAddress{targetAddress},
                                           mUserData{std::move(userData)}
    {
    }

    void DiagMessage::GetPayload(std::vector<uint8_t> &payload) const
    {
        payload.insert(payload.begin(), mUserData.cbegin(), mUserData.cend());
        Convert::ToByteVector<uint16_t>(mTargetAddress, payload);
        Convert::ToByteVector<uint16_t>(mSourceAddress, payload);
    }

    bool DiagMessage::TrySetPayload(const std::vector<uint8_t> &payload)
    {
        std::size_t _offset{cHeaderSize};

        if (payload.size() > cHeaderSize)
        {
            mSourceAddress =
                Convert::ToUnsignedInteger<uint16_t>(payload, _offset);

            mTargetAddress =
                Convert::ToUnsignedInteger<uint16_t>(payload, _offset);

            auto _beginItr{payload.cbegin() + _offset};
            mUserData = std::vector<uint8_t>(_beginItr, payload.cend());

            return true;
        }
        else
        {
            return false;
        }
    }

    uint16_t DiagMessage::GetSourceAddress() const noexcept
    {
        return mSourceAddress;
    }

    uint16_t DiagMessage::GetTargetAddress() const noexcept
    {
        return mTargetAddress;
    }

    void DiagMessage::GetUserData(std::vector<uint8_t> &userData)
    {
        userData = mUserData;
    }
}