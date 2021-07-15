#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

namespace ara
{
    namespace log
    {
        enum class LogLevel : std::uint8_t
        {
            kOff = 0x00,
            kFatal = 0x01,
            kError = 0x02,
            kWarn = 0x03,
            kInfo = 0x04,
            kDebug = 0x05,
            kVerbose = 0x06
        };

        enum class LogMode : std::uint8_t
        {
            kRemote = 0x01,
            kFile = 0x02,
            kConsole = 0x04
        };

        enum class ClientState : std::int8_t
        {
            kUnknown = -1,
            kNotConnected = 0,
            kConnected = 1
        };
    }
}

#endif