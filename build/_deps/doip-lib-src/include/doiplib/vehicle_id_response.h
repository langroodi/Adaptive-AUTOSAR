#ifndef VEHICLE_ID_RESPONSE_H
#define VEHICLE_ID_RESPONSE_H

#include <array>
#include <string>
#include "./message.h"

namespace DoipLib
{
    /// @brief Vehicle announcement ID response message
    class VehicleIdResponse : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::VehicleAnnoucementIdResponse};
        static const std::size_t cVinSize{17};
        static constexpr std::size_t cIdSize{6};

        std::string mVin;
        uint16_t mLogicalAddress;
        std::array<uint8_t, cIdSize> mEid;
        std::array<uint8_t, cIdSize> mGid;
        uint8_t mFurtherAction;
        bool mUseVehicleIdSync;
        uint8_t mVinGidStatus;

        void SetPayload(const std::vector<uint8_t> &payload);

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;

    public:
        VehicleIdResponse() noexcept;

        /// @brief Constructor without optional VIN/GID synchronization
        /// @param protocolVersion DoIP ISO protocol version
        /// @param vin Vehicle ID number (VIN)
        /// @param logicalAddress Entity logical address
        /// @param eid Entity ID
        /// @param gid Group ID (GID)
        /// @param furtherAction Further action byte
        /// @throws std::out_of_range Throws if the given VIN is invalid
        VehicleIdResponse(
            uint8_t protocolVersion,
            const std::string &vin,
            uint16_t logicalAddress,
            std::array<uint8_t, cIdSize> eid,
            std::array<uint8_t, cIdSize> gid,
            uint8_t furtherAction);

        /// @brief Constructor without optional VIN/GID synchronization
        /// @param protocolVersion DoIP ISO protocol version
        /// @param vin Vehicle ID number (VIN)
        /// @param logicalAddress Entity logical address
        /// @param eid Entity ID
        /// @param gid Group ID (GID)
        /// @param furtherAction Further action byte
        /// @throws std::out_of_range Throws if the given VIN is invalid
        VehicleIdResponse(
            uint8_t protocolVersion,
            std::string &&vin,
            uint16_t logicalAddress,
            std::array<uint8_t, cIdSize> eid,
            std::array<uint8_t, cIdSize> gid,
            uint8_t furtherAction);

        /// @brief Constructor using VIN/GID synchronization
        /// @param protocolVersion DoIP ISO protocol version
        /// @param vin Vehicle ID number (VIN)
        /// @param logicalAddress Entity logical address
        /// @param eid Entity ID
        /// @param gid Group ID (GID)
        /// @param furtherAction Further action byte
        /// @param vinGidStatus VIN/GID synchronization status
        /// @throws std::out_of_range Throws if the given VIN is invalid
        VehicleIdResponse(
            uint8_t protocolVersion,
            const std::string &vin,
            uint16_t logicalAddress,
            std::array<uint8_t, cIdSize> eid,
            std::array<uint8_t, cIdSize> gid,
            uint8_t furtherAction,
            uint8_t vinGidStatus);

        /// @brief Constructor using VIN/GID synchronization
        /// @param protocolVersion DoIP ISO protocol version
        /// @param vin Vehicle ID number (VIN)
        /// @param logicalAddress Entity logical address
        /// @param eid Entity ID
        /// @param gid Group ID (GID)
        /// @param furtherAction Further action byte
        /// @param vinGidStatus VIN/GID synchronization status
        /// @throws std::out_of_range Throws if the given VIN is invalid
        VehicleIdResponse(
            uint8_t protocolVersion,
            std::string &&vin,
            uint16_t logicalAddress,
            std::array<uint8_t, cIdSize> eid,
            std::array<uint8_t, cIdSize> gid,
            uint8_t furtherAction,
            uint8_t vinGidStatus);

        /// @brief Get VIN
        /// @return Vehicle ID number
        std::string GetVin() const;

        /// @brief Get logical address
        /// @return Entity logical address
        uint16_t GetLogicalAddress() const noexcept;

        /// @brief Get EID
        /// @return Entity ID
        std::array<uint8_t, cIdSize> GetEid() const;

        /// @brief Get GID
        /// @return Group ID
        std::array<uint8_t, cIdSize> GetGid() const;

        /// @brief Get further action
        /// @return Further action byte
        uint8_t GetFurtherAction() const noexcept;

        /// @brief Try to get the VIN/GIN status
        /// @param[out] vinGinStatus VIN/GIN synchronization status
        /// @return True if the VIN/GIN synchronization status is being used, otherwise false
        bool TryGetVinGinStatus(uint8_t &vinGinStatus) const noexcept;
    };
}

#endif