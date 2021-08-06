#ifndef LOG_STREAM_H
#define LOG_STREAM_H

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
        /// @brief A stream pipeline to combine log entities
        class LogStream final
        {
        private:
            std::string mLogs;
            void concat(std::string &&log);

        public:
            /// @brief Clear the stream
            void Flush() noexcept;

            /// @brief Arugment insertion operator
            /// @tparam T Argument playload type
            /// @param arg An agrgument
            /// @returns Reference to the current log stream
            template <typename T>
            LogStream &operator<<(const Argument<T> &arg)
            {
                std::string _argumentString = arg.ToString();
                concat(std::move(_argumentString));

                return *this;
            }

            /// @brief LogStream insertion operator
            /// @param value Another logstream
            /// @returns Reference to the current log stream
            LogStream &operator<<(const LogStream &value);

            /// @brief Boolean insertion operator
            /// @param value A boolean value
            /// @returns Reference to the current log stream
            LogStream &operator<<(bool value);

            /// @brief Byte insertion operator
            /// @param value A byte value
            /// @returns Reference to the current log stream
            LogStream &operator<<(uint8_t value);

            /// @brief Unsigned integer insertion operator
            /// @param value An unsigned integer value
            /// @returns Reference to the current log stream
            LogStream &operator<<(uint32_t value);

            /// @brief Float insertion operator
            /// @param value A float value
            /// @returns Reference to the current log stream
            LogStream &operator<<(float value);

            /// @brief String insertion operator
            /// @param value A string
            /// @returns Reference to the current log stream
            LogStream &operator<<(const std::string &value);

            /// @brief C-syle string insertion operator
            /// @param value Character array
            /// @returns Reference to the current log stream
            LogStream &operator<<(const char *value);

            /// @brief LogLeve insertion operator
            /// @param value Log severity level
            /// @returns Reference to the current log stream
            LogStream &operator<<(LogLevel value);

            /// @brief ErrorCode insertion operator
            /// @param value An error code object
            /// @returns Reference to the current log stream
            LogStream &operator<<(const ara::core::ErrorCode &value);

            /// @brief InstanceSpecifier insertion operator
            /// @param value An instance specifier object
            /// @returns Reference to the current log stream
            LogStream &operator<<(const ara::core::InstanceSpecifier &value) noexcept;

            /// @brief Data array insertion operator
            /// @param value Data byte vector
            /// @returns Reference to the current log stream
            LogStream &operator<<(std::vector<std::uint8_t> value);

            /// @brief Log stream at a certian file and a certian line within the file
            /// @param file File name
            /// @param line Line number
            /// @returns Reference to the current log stream
            LogStream &WithLocation(std::string file, int line);

            /// @brief Convert the current log stream to a standard string
            /// @returns Serialized log stream string
            std::string ToString() const noexcept;
        };
    }
}

#endif