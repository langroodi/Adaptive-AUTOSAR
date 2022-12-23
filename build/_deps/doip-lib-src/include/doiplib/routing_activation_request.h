#ifndef ROUTING_ACTIVATION_REQUEST_H
#define ROUTING_ACTIVATION_REQUEST_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Tester routing activation request to an internal vehicle network
    class RoutingActivationRequest : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::RoutingActivationRequest};
        const uint32_t cIsoReserved{0x00000000};

        uint16_t mSourceAddress;
        uint8_t mActivationType;
        bool mHasOemSpecificData;
        uint32_t mOemSpecificData;

        RoutingActivationRequest(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint8_t activationType,
            bool hasOemSpecificData,
            uint32_t oemSpecificData = 0) noexcept;

        bool TrySetCompulsoryPayload(const std::vector<uint8_t> &payload);

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;

    public:
        RoutingActivationRequest() noexcept;

        /// @brief Constructor without OEM-specific data
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress External tester logical address
        /// @param activationType Routing activation type
        RoutingActivationRequest(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint8_t activationType) noexcept;

        /// @brief Constructor using OEM-specific data
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress External tester logical address
        /// @param activationType Routing activation type
        /// @param oemSpecificData OEM-specific request data
        RoutingActivationRequest(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint8_t activationType,
            uint32_t oemSpecificData) noexcept;

        /// @brief Get request source address
        /// @return External tester logical address
        uint16_t GetSourceAddress() const noexcept;

        /// @brief Get activation type
        /// @return Routing activation type
        uint8_t GetActivationType() const noexcept;

        /// @brief Try to get the OEM-specific data
        /// @param[out] oemSpecificData OEM-specific request data
        /// @return True if the data is available, otherwise false
        bool TryGetOemSpecificData(uint32_t &oemSpecificData) const noexcept;
    };
}

#endif