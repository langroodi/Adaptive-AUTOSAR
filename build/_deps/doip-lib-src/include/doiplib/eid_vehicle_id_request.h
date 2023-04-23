#ifndef EID_VEHICLE_ID_REQUEST_H
#define EID_VEHICLE_ID_REQUEST_H

#include <array>
#include "./message.h"

namespace DoipLib
{
    /// @brief Vehicle ID request DoIP message with entity ID filtering
    class EidVehicleIdRequest : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::VehicleIdRequestWithEid};
        static constexpr std::size_t cEidSize{6};

        std::array<uint8_t, cEidSize> mEid;

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(
            const std::vector<uint8_t> &payload,
            uint32_t payloadLength) override;

    public:
        EidVehicleIdRequest() noexcept;

        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param eid Entity ID filter
        EidVehicleIdRequest(
            uint8_t protocolVersion, std::array<uint8_t, cEidSize> eid);

        /// @brief Get EID filter
        /// @return Entity ID
        std::array<uint8_t, cEidSize> GetEid() const;
    };
}

#endif