#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

namespace ara
{
    /// @brief Adaptive AUTOSAR Logging
    namespace log
    {
        /// @brief Log severity level
        enum class LogLevel : std::uint8_t
        {
            kOff = 0x00,        ///< No logging
            kFatal = 0x01,      ///< Fatal log
            kError = 0x02,      ///< Error log
            kWarn = 0x03,       ///< Warning log
            kInfo = 0x04,       ///< Informative log
            kDebug = 0x05,      ///< Debug log
            kVerbose = 0x06     ///< Verbose log
        };

        /// @brief Log sink mode
        enum class LogMode : std::uint8_t
        {
            kRemote = 0x01,     ///< Remote network logging sink
            kFile = 0x02,       ///< File logging sink for debugging
            kConsole = 0x04     ///< Console logging sink for debugging
        };

        /// @brief Logging client connection state
        enum class ClientState : std::int8_t
        {
            kUnknown = -1,      ///< Connection state unknown
            kNotConnected = 0,  ///< Client is disconnected
            kConnected = 1      ///< Client is connected
        };
    }
}

#endif