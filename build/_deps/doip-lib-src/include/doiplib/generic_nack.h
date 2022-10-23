#ifndef GENERIC_NACK_H
#define GENERIC_NACK_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Generic negative acknowledgement DoIP message
    class GenericNack : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::GenericNegativeAcknowledgement};

        GenericNackType mNackCode;

    protected:
         void GetPayload(std::vector<uint8_t> &payload) const override;

         virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;
         
    public:
        GenericNack() noexcept;
        
        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param nackCode Negative acknowledgement code
        GenericNack(uint8_t protocolVersion, GenericNackType nackCode) noexcept;

        /// @brief Get NACK code
        /// @return Negative acknoweledgement code
        GenericNackType GetNackCode() const noexcept;
    };
}

#endif