#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <stdint.h>
#include <string_view>
#include <vector>

#include "../core/error_code.h"
#include "../core/instance_specifier.h"
#include "./common.h"
#include "./argument.h"

namespace ara
{
    namespace log
    {
        class LogStream final
        {
        public:
            constexpr LogStream() noexcept;
            ~LogStream() noexcept;
            void Flush() noexcept;
            template <typename T>
            LogStream &operator<<(const Argument<T> &arg) noexcept;
            LogStream &operator<<(bool value) noexcept;
            LogStream &operator<<(uint8_t value) noexcept;
            LogStream &operator<<(uint32_t value) noexcept;
            LogStream &operator<<(float value) noexcept;
            LogStream &operator<<(const std::string_view &value) noexcept;
            LogStream &operator<<(const char *value) noexcept;
            LogStream &operator<<(LogStream &out, LogLevel value) noexcept;
            LogStream &operator<<(
                LogStream &out,
                const ara::core::ErrorCode &ec) noexcept;
            LogStream &operator<<(
                LogStream &out,
                const ara::core::InstanceSpecifier &value) noexcept;
            LogStream &operator<<(std::vector<std::uint8_t> data) noexcept;
            LogStream &WithLocation(std::string_view file, int line) noexcept;
        };
    }
}

#endif