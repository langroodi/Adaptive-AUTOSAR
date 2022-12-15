#ifndef VIN_VEHICLE_ID_REQUEST_H
#define VIN_VEHICLE_ID_REQUEST_H

#include <string>
#include "./message.h"

namespace DoipLib
{
    /// @brief Vehicle ID request DoIP message with vehicle ID number filtering
    class VinVehicleIdRequest : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::VehicleIdRequestWithVin};
        static const std::size_t cVinSize{17};

        std::string mVin;

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;

    public:
        VinVehicleIdRequest() noexcept;

        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param vin Vehicle ID number filter
        /// @throws std::out_of_range Throws if the given VIN is invalid
        VinVehicleIdRequest(uint8_t protocolVersion, const std::string &vin);

        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param vin Vehicle ID number filter
        /// @throws std::out_of_range Throws if the given VIN is invalid
        VinVehicleIdRequest(uint8_t protocolVersion, std::string &&vin);

        /// @brief Get VIN filter
        /// @return Vehicle ID number
        std::string GetVin() const;
    };
}

#endif