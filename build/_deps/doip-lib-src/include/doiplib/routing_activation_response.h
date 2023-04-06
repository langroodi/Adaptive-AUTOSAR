#ifndef ROUTING_ACTIVATION_RESPONSE_H
#define ROUTING_ACTIVATION_RESPONSE_H

#include "./message.h"
#include "./routing_activation_response_type.h"

namespace DoipLib
{
    /// @brief Tester routing activation response to an internal vehicle network
    class RoutingActivationResponse : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::RoutingActivationResponse};
        const uint32_t cIsoReserved{0x00000000};

        uint16_t mTesterLogicalAddress;
        uint16_t mEntityLogicalAddress;
        RoutingActivationResponseType mResponseCode;
        bool mHasOemSpecificData;
        uint32_t mOemSpecificData;

        RoutingActivationResponse(
            uint8_t protocolVersion,
            uint16_t testerLogicalAddress,
            uint16_t entityLogicalAddress,
            RoutingActivationResponseType responseCode,
            bool hasOemSpecificData,
            uint32_t oemSpecificData = 0) noexcept;

        void SetPayload(const std::vector<uint8_t> &payload);

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(
            const std::vector<uint8_t> &payload,
            uint32_t payloadLength) override;

    public:
        RoutingActivationResponse() noexcept;

        /// @brief Constructor without OEM-specific data
        /// @param protocolVersion DoIP ISO protocol version
        /// @param testerLogicalAddress External tester logical address
        /// @param entityLogicalAddress Internal DoIP entity logical address
        /// @param responseCode Routing activation response code
        RoutingActivationResponse(
            uint8_t protocolVersion,
            uint16_t testerLogicalAddress,
            uint16_t entityLogicalAddress,
            RoutingActivationResponseType responseCode) noexcept;

        /// @brief Constructor using OEM-specific data
        /// @param protocolVersion DoIP ISO protocol version
        /// @param testerLogicalAddress External tester logical address
        /// @param entityLogicalAddress Internal DoIP entity logical address
        /// @param responseCode Routing activation response code
        /// @param oemSpecificData OEM-specific request data
        RoutingActivationResponse(
            uint8_t protocolVersion,
            uint16_t testerLogicalAddress,
            uint16_t entityLogicalAddress,
            RoutingActivationResponseType responseCode,
            uint32_t oemSpecificData) noexcept;

        /// @brief Get tester logical address
        /// @return External tester logical address
        uint16_t GetTesterLogicalAddress() const noexcept;

        /// @brief Get entity logical address
        /// @return Internal DoIP entity logical address
        uint16_t GetEntityLogicalAddress() const noexcept;

        /// @brief Get response code
        /// @return Routing activation response code
        RoutingActivationResponseType GetResponseCode() const noexcept;

        /// @brief Try to get the OEM-specific data
        /// @param[out] oemSpecificData OEM-specific request data
        /// @return True if the data is available, otherwise false
        bool TryGetOemSpecificData(uint32_t &oemSpecificData) const noexcept;
    };
}

#endif