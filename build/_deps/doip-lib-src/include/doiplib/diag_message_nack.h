#ifndef DIAG_MESSAGE_ACK_H
#define DIAG_MESSAGE_ACK_H

#include "./message.h"
#include "./diag_nack_type.h"

namespace DoipLib
{
    /// @brief Diagnostic message negative acknowledgement
    class DiagMessageNack : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::DiagMessageNegativeAcknowledgement};

        uint16_t mSourceAddress;
        uint16_t mTargetAddress;
        DiagNackType mNackCode;
        bool mHasPreviousMessage;
        std::vector<uint8_t> mPreviousMessage;

        void SetCompulsoryPayload(const std::vector<uint8_t> &payload);

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;

    public:
        DiagMessageNack();

        /// @brief Constructor without previous message
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param nackCode Negative acknowledgement code
        DiagMessageNack(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            DiagNackType nackCode);

        /// @brief Constructor including previous message
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param nackCode Negative acknowledgement code
        /// @param previousMessage Previous diagnostic message payload
        DiagMessageNack(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            DiagNackType nackCode,
            const std::vector<uint8_t> &previousMessage);

        /// @brief Constructor including previous message
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param nackCode Negative acknowledgement code
        /// @param previousMessage Previous diagnostic message payload
        DiagMessageNack(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            DiagNackType nackCode,
            std::vector<uint8_t> &&previousMessage);

        /// @brief Get message source address
        /// @return Sender logical address
        uint16_t GetSourceAddress() const noexcept;

        /// @brief Get message target address
        /// @return Receiver logical address
        uint16_t GetTargetAddress() const noexcept;

        /// @brief Get NACK code
        /// @return Negative acknowledgement code
        DiagNackType GetNackCode() const noexcept;

        /// @brief Get previous diagnostic message
        /// @param[out] previousMessage Previous diagnostic message payload
        /// @return True if the message is available, otherwise false
        bool TryGetPreviousMessage(std::vector<uint8_t> &previousMessage);
    };
}

#endif