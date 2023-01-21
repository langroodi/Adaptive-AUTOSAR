#ifndef DIAG_NACK_TYPE_H
#define DIAG_NACK_TYPE_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief Diagnostic message negative acknowledgement type
    enum class DiagNackType : uint8_t
    {
        InvalidSourceAddress = 0x02,    ///!< Invalid message source logical address
        InvalidTargetAddress = 0x03,    ///!< Invalid message target logical address
        TooLargeMessage = 0x04,         ///!< Message with payload length larger than 'DoIPMaxRequest'
        ReceiveBufferOverflow = 0x05,   ///!< Message with a size larger than the receive buffer
        UnreachableTarget = 0x06,       ///!< Unreachable message target due to lack of routing activation
        UnknownNetwork = 0x07,          ///!< Message rejection due to unknown network error
        TransportProtocolError = 0x08   ///!< Message rejection due to transport protocol error
    };
}

#endif