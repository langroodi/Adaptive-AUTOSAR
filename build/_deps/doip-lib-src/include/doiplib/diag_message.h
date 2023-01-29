#ifndef DIAG_MESSAGE_H
#define DIAG_MESSAGE_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Diagnostic message (e.g., an UDS message)
    class DiagMessage : public Message
    {
    private:
        static const PayloadType cPayloadType{PayloadType::DiagMessage};

        uint16_t mSourceAddress;
        uint16_t mTargetAddress;
        std::vector<uint8_t> mUserData;

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;

    public:
        DiagMessage();

        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param userData Actual diagnostic payload data (e.g., UDS message payload)
        DiagMessage(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            const std::vector<uint8_t> &userData);

        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param userData Actual diagnostic payload data (e.g., UDS message payload)
        DiagMessage(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            std::vector<uint8_t> &&userData);

        /// @brief Get message source address
        /// @return Sender logical address
        uint16_t GetSourceAddress() const noexcept;

        /// @brief Get message target address
        /// @return Receiver logical address
        uint16_t GetTargetAddress() const noexcept;

        /// @brief Get user data
        /// @param[out] userData Actual diagnostic payload data (e.g., UDS message payload)
        void GetUserData(std::vector<uint8_t> &userData);
    };
}

#endif