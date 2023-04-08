#ifndef GENERIC_NACK_TYPE_H
#define GENERIC_NACK_TYPE_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief Generic negative acknowledgement type
    enum class GenericNackType : uint8_t
    {
        InvalidProtocolVersion = 0x00,  ///!< Invalid DoIP protocol information
        UnsupportedPayloadType = 0x01,  ///!< Unsupported DoIP payload type
        ToolLargeMessage = 0x02,        ///!< Payload length exceeding the 'DoIPMaxRequestBytes'
        ReceiveBufferOverflow = 0x03,   ///!< Payload length exceeding the available buffer size
        InvalidPayloadLength = 0x04,    ///!< Invalid payload length for the specified payload type
    };
}

#endif