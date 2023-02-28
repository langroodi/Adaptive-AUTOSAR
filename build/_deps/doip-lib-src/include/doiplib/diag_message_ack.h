#ifndef DIAG_MESSAGE_ACK_H
#define DIAG_MESSAGE_ACK_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Diagnostic message positive acknowledgement
    class DiagMessageAck : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::DiagMessagePositiveAcknowledgement};
        const uint8_t cAckCode{0x00};

        uint16_t mSourceAddress;
        uint16_t mTargetAddress;
        bool mHasPreviousMessage;
        std::vector<uint8_t> mPreviousMessage;

        bool TrySetCompulsoryPayload(const std::vector<uint8_t> &payload);

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;

    public:
        DiagMessageAck();

        /// @brief Constructor without previous message
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        DiagMessageAck(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress);

        /// @brief Constructor including previous message
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param previousMessage Previous diagnostic message payload
        DiagMessageAck(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            const std::vector<uint8_t> &previousMessage);

        /// @brief Constructor including previous message
        /// @param protocolVersion DoIP ISO protocol version
        /// @param sourceAddress Sender logical address
        /// @param targetAddress Receiver logical address
        /// @param previousMessage Previous diagnostic message payload
        DiagMessageAck(
            uint8_t protocolVersion,
            uint16_t sourceAddress,
            uint16_t targetAddress,
            std::vector<uint8_t> &&previousMessage);

        /// @brief Get message source address
        /// @return Sender logical address
        uint16_t GetSourceAddress() const noexcept;

        /// @brief Get message target address
        /// @return Receiver logical address
        uint16_t GetTargetAddress() const noexcept;

        /// @brief Get previous diagnostic message
        /// @param[out] previousMessage Previous diagnostic message payload
        /// @return True if the message is available, otherwise false
        bool TryGetPreviousMessage(std::vector<uint8_t> &previousMessage);
    };
}

#endif