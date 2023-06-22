#ifndef VEHICLE_ID_REQUEST_H
#define VEHICLE_ID_REQUEST_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Vehicle ID request DoIP message
    class VehicleIdRequest : public Message
    {
    private:
        static const PayloadType cPayloadType{PayloadType::VehicleIdRequest};

    protected:
         void GetPayload(std::vector<uint8_t> &payload) const override;

         virtual bool TrySetPayload(
            const std::vector<uint8_t> &payload,
            uint32_t payloadLength) override;
         
    public:
        VehicleIdRequest() noexcept;
        
        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        explicit VehicleIdRequest(uint8_t protocolVersion) noexcept;
    };
}

#endif