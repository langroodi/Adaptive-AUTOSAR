#ifndef ENTITY_STATUS_RESPONSE_H
#define ENTITY_STATUS_RESPONSE_H

#include "./message.h"
#include "./node_type.h"

namespace DoipLib
{
    /// @brief Diagnostic entity status response
    class EntityStatusResponse : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::DoipEntityStatusResponse};

        NodeType mEntityType;
        uint8_t mMaxOpenSockets;
        uint8_t mCurrentlyOpenSocket;
        bool mHasMaxDataSize;
        uint32_t mMaxDataSize;

        EntityStatusResponse(
            uint8_t protocolVersion,
            NodeType entityType,
            uint8_t maxOpenSockets,
            uint8_t currentlyOpenSocket,
            bool hasMaxDataSize,
            uint32_t maxDataSize = 0) noexcept;

        void SetPayload(const std::vector<uint8_t> &payload);

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override;

        virtual bool TrySetPayload(
            const std::vector<uint8_t> &payload,
            uint32_t payloadLength) override;

    public:
        EntityStatusResponse() noexcept;

        /// @brief Constructor without maximum data size field
        /// @param protocolVersion DoIP ISO protocol version
        /// @param entityType DoIP entity type
        /// @param maxOpenSockets Maximum number of concurrent open TCP sockets at the entity
        /// @param currentlyOpenSocket Current number of open TCP socket at the entity
        EntityStatusResponse(
            uint8_t protocolVersion,
            NodeType entityType,
            uint8_t maxOpenSockets,
            uint8_t currentlyOpenSocket) noexcept;

        /// @brief Constructor using maximum data size field
        /// @param protocolVersion DoIP ISO protocol version
        /// @param entityType DoIP entity type
        /// @param maxOpenSockets Maximum number of concurrent open TCP sockets at the entity
        /// @param currentlyOpenSocket Current number of open TCP socket at the entity
        /// @param maxDataSize Maximum processable request data size
        EntityStatusResponse(
            uint8_t protocolVersion,
            NodeType entityType,
            uint8_t maxOpenSockets,
            uint8_t currentlyOpenSocket,
            uint32_t maxDataSize) noexcept;

        /// @brief Get entity type
        /// @return DoIP entity type
        NodeType GetEntityType() const noexcept;

        /// @brief Get maximum open sockets number
        /// @return Maximum number of concurrent open TCP sockets at the entity
        uint8_t GetMaxOpenSockets() const noexcept;

        /// @brief Get currently open socket number
        /// @return Current number of open TCP socket at the entity
        uint8_t GetCurrentlyOpenSocket() const noexcept;

        /// @brief Try to get the maximum data size
        /// @param[out] maxDataSize Maximum processable request data size
        /// @return True if the maximum size is available, otherwise false
        bool TryGetMaxDataSize(uint32_t &maxDataSize) const noexcept;
    };
}

#endif