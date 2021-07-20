#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <string>
#include <stdint.h>
#include <vector>
#include <utility>

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
        private:
            std::string mLogs;
            constexpr void concat(std::string &&log)
            {
                mLogs += log;
            }

        public:
            void Flush() noexcept;
            
            template <typename T>
            LogStream &operator<<(const Argument<T> &arg)
            {
                std::string _argumentString = arg.ToString();
                concat(std::move(_argumentString));

                return *this;
            }

            LogStream &operator<<(bool value);
            LogStream &operator<<(uint8_t value);
            LogStream &operator<<(uint32_t value);
            LogStream &operator<<(float value);
            LogStream &operator<<(const std::string &value);
            LogStream &operator<<(const char *value);
            LogStream &operator<<(LogStream &out, LogLevel value) noexcept;
            LogStream &operator<<(
                LogStream &out,
                const ara::core::ErrorCode &ec) noexcept;
            LogStream &operator<<(
                LogStream &out,
                const ara::core::InstanceSpecifier &value) noexcept;
            LogStream &operator<<(std::vector<std::uint8_t> data) noexcept;
            LogStream &WithLocation(std::string file, int line) noexcept;
        };
    }
}

#endif