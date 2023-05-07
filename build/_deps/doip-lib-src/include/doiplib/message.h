#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include "./payload_type.h"
#include "./generic_nack_type.h"

namespace DoipLib
{
    /// @brief DoIP generic message
    class Message
    {
    private:
        uint8_t mProtocolVersion;
        const PayloadType mPayloadType;

    protected:
        /// @brief DoIP message header size
        static const std::size_t cHeaderSize{8};

        /// @brief Constructor
        /// @param payloadType DoIP message payload type
        explicit Message(PayloadType payloadType) noexcept;

        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        /// @param payloadType DoIP message payload type
        Message(uint8_t protocolVersion, PayloadType payloadType) noexcept;

        /// @brief Get message payload
        /// @param[out] payload Payload byte array
        virtual void GetPayload(std::vector<uint8_t> &payload) const = 0;

        /// @brief Try to set message payload
        /// @param[in] payload Payload byte array
        /// @param[in] payloadLength Payload length from the message header
        /// @return True if the payload is set successfully, otherwise false
        virtual bool TrySetPayload(
            const std::vector<uint8_t> &payload,
            uint32_t payloadLength) = 0;

    public:
        /// @brief Serialize DoIP message
        /// @param[out] serializedMessage Serialize message byte array
        void Serialize(std::vector<uint8_t> &serializedMessage) const;

        /// @brief Try to deserialize DoIP message
        /// @param[in] serializedMessage Serialize message byte array
        /// @param[out] nackCode Negative acknowledgement code if deserialization failed
        /// @return True if the deserialization is successful, otherwise false
        bool TryDeserialize(
            const std::vector<uint8_t> &serializedMessage,
            GenericNackType &nackCode);

        /// @brief Try to extract the payload type from the given serialized message
        /// @param[in] serializedMessage Serialized message byte array
        /// @param[out] payloadType Extracted payload type
        /// @return True if the payload type is extracted successfully, otherwise false
        static bool TryExtractPayloadType(
            const std::vector<uint8_t> &serializedMessage,
            PayloadType &payloadType);

        virtual ~Message() noexcept = default;
    };
}

#endif