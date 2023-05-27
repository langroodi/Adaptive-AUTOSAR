#include <stdexcept>
#include <utility>
#include "doiplib/vehicle_id_response.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    const PayloadType VehicleIdResponse::cPayloadType;
    const std::size_t VehicleIdResponse::cVinSize;
    constexpr std::size_t VehicleIdResponse::cIdSize;

    VehicleIdResponse::VehicleIdResponse() noexcept : Message(cPayloadType)
    {
    }

    VehicleIdResponse::VehicleIdResponse(
        uint8_t protocolVersion,
        const std::string &vin,
        uint16_t logicalAddress,
        std::array<uint8_t, VehicleIdResponse::cIdSize> eid,
        std::array<uint8_t, VehicleIdResponse::cIdSize> gid,
        uint8_t furtherAction) : Message(protocolVersion, cPayloadType),
                                 mVin{vin.size() == cVinSize ? vin : throw std::out_of_range("Invalid vehicle ID number")},
                                 mLogicalAddress{logicalAddress},
                                 mEid{eid},
                                 mGid{gid},
                                 mFurtherAction{furtherAction},
                                 mUseVehicleIdSync{false}
    {
    }

    VehicleIdResponse::VehicleIdResponse(
        uint8_t protocolVersion,
        std::string &&vin,
        uint16_t logicalAddress,
        std::array<uint8_t, VehicleIdResponse::cIdSize> eid,
        std::array<uint8_t, VehicleIdResponse::cIdSize> gid,
        uint8_t furtherAction) : Message(protocolVersion, cPayloadType),
                                 mVin{vin.size() == cVinSize ? std::move(vin) : throw std::out_of_range("Invalid vehicle ID number")},
                                 mLogicalAddress{logicalAddress},
                                 mEid{eid},
                                 mGid{gid},
                                 mFurtherAction{furtherAction},
                                 mUseVehicleIdSync{false}
    {
    }

    VehicleIdResponse::VehicleIdResponse(
        uint8_t protocolVersion,
        const std::string &vin,
        uint16_t logicalAddress,
        std::array<uint8_t, VehicleIdResponse::cIdSize> eid,
        std::array<uint8_t, VehicleIdResponse::cIdSize> gid,
        uint8_t furtherAction,
        uint8_t vinGidStatus) : Message(protocolVersion, cPayloadType),
                                mVin{vin.size() == cVinSize ? vin : throw std::out_of_range("Invalid vehicle ID number")},
                                mLogicalAddress{logicalAddress},
                                mEid{eid},
                                mGid{gid},
                                mFurtherAction{furtherAction},
                                mUseVehicleIdSync{true},
                                mVinGidStatus{vinGidStatus}
    {
    }

    VehicleIdResponse::VehicleIdResponse(
        uint8_t protocolVersion,
        std::string &&vin,
        uint16_t logicalAddress,
        std::array<uint8_t, VehicleIdResponse::cIdSize> eid,
        std::array<uint8_t, VehicleIdResponse::cIdSize> gid,
        uint8_t furtherAction,
        uint8_t vinGidStatus) : Message(protocolVersion, cPayloadType),
                                mVin{vin.size() == cVinSize ? std::move(vin) : throw std::out_of_range("Invalid vehicle ID number")},
                                mLogicalAddress{logicalAddress},
                                mEid{eid},
                                mGid{gid},
                                mFurtherAction{furtherAction},
                                mUseVehicleIdSync{true},
                                mVinGidStatus{vinGidStatus}
    {
    }

    void VehicleIdResponse::GetPayload(std::vector<uint8_t> &payload) const
    {
        if (mUseVehicleIdSync)
        {
            payload.insert(payload.begin(), mVinGidStatus);
        }

        payload.insert(payload.begin(), mFurtherAction);
        payload.insert(payload.begin(), mGid.cbegin(), mGid.cend());
        payload.insert(payload.begin(), mEid.cbegin(), mEid.cend());
        Convert::ToByteVector<uint16_t>(mLogicalAddress, payload);
        payload.insert(payload.begin(), mVin.cbegin(), mVin.cend());
    }

    void VehicleIdResponse::SetPayload(const std::vector<uint8_t> &payload)
    {
        std::size_t _offset{cHeaderSize};

        mVin = Convert::ToString<cVinSize>(payload, _offset);
        mLogicalAddress = Convert::ToUnsignedInteger<uint16_t>(payload, _offset);
        mEid = Convert::ToByteArray<cIdSize>(payload, _offset);
        mGid = Convert::ToByteArray<cIdSize>(payload, _offset);
        mFurtherAction = payload.at(_offset);
    }

    bool VehicleIdResponse::TrySetPayload(
        const std::vector<uint8_t> &payload,
        uint32_t payloadLength)
    {
        const std::size_t cExpectedPayloadSizeMin{32};
        const std::size_t cExpectedPayloadSizeMax{33};

        if (payloadLength == cExpectedPayloadSizeMin)
        {
            SetPayload(payload);
            // No vehicle ID synchronization
            mUseVehicleIdSync = false;

            return true;
        }
        else if (payloadLength == cExpectedPayloadSizeMax)
        {
            const std::size_t cVinGidStatusIndex{
                static_cast<std::size_t>(cHeaderSize + cExpectedPayloadSizeMin)};

            SetPayload(payload);
            // Use vehicle ID synchronization
            mUseVehicleIdSync = true;
            mVinGidStatus = payload.at(cVinGidStatusIndex);

            return true;
        }
        else
        {
            return false;
        }
    }

    std::string VehicleIdResponse::GetVin() const
    {
        return mVin;
    }

    uint16_t VehicleIdResponse::GetLogicalAddress() const noexcept
    {
        return mLogicalAddress;
    }

    std::array<uint8_t, VehicleIdResponse::cIdSize> VehicleIdResponse::GetEid() const
    {
        return mEid;
    }

    std::array<uint8_t, VehicleIdResponse::cIdSize> VehicleIdResponse::GetGid() const
    {
        return mGid;
    }

    uint8_t VehicleIdResponse::GetFurtherAction() const noexcept
    {
        return mFurtherAction;
    }

    bool VehicleIdResponse::TryGetVinGinStatus(uint8_t &vinGinStatus) const noexcept
    {
        if (mUseVehicleIdSync)
        {
            vinGinStatus = mVinGidStatus;
            return true;
        }
        else
        {
            return false;
        }
    }
}