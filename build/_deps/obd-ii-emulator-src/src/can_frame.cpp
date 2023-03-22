#include <stdexcept>
#include <algorithm>
#include "../include/obdemulator/can_frame.h"

namespace ObdEmulator
{
    CanFrame::CanFrame(
        uint32_t id,
        bool extended,
        bool remote,
        const std::vector<uint8_t> &data)
    {
        if (extended && id > cEXtendedIdMax)
        {
            throw std::out_of_range("Extended ID is out of range.");
        }
        else if (!extended && id > cStandardIdMax)
        {
            throw std::out_of_range("Standard ID is out of range.");
        }

        mId = id;
        mExtended = extended;
        mRemote = remote;
        mDataLength = data.size();
        std::copy(data.cbegin(), data.cend(), mData.begin());
    }

    u_int32_t CanFrame::GetId() const noexcept
    {
        return mId;
    }

    bool CanFrame::IsExtended() const noexcept
    {
        return mExtended;
    }

    bool CanFrame::IsRemote() const noexcept
    {
        return mRemote;
    }

    size_t CanFrame::GetDataLength() const noexcept
    {
        return mDataLength;
    }

    const std::array<uint8_t, CanFrame::cDataLengthMax> &CanFrame::GetData() const noexcept
    {
        return mData;
    }
}