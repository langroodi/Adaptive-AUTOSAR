#ifndef POWER_MODE_RESPONSE_H
#define POWER_MODE_RESPONSE_H

#include "./message.h"
#include "./power_mode_type.h"

namespace DoipLib
{
    /// @brief Diagnostic node power mode information response
    class PowerModeResponse : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::DiagPowerModeInfoResponse};

        PowerModeType mPowerMode;

    protected:
         void GetPayload(std::vector<uint8_t> &payload) const override;

         virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;
         
    public:
        PowerModeResponse() noexcept;
        
        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param powerMode DoIP node current power mode
        explicit PowerModeResponse(
            uint8_t protocolVersion, PowerModeType powerMode) noexcept;

        /// @brief Get power mode
        /// @return DoIP node current power mode
        PowerModeType GetPowerMode() const noexcept;
    };
}

#endif