#ifndef LOGGER_H
#define LOGGER_H

#include "./log_stream.h"

namespace ara
{
    namespace log
    {
        /// @brief Logger of a specific context
        class Logger
        {
        private:
            std::string mContextId;
            std::string mContextDescription;
            LogLevel mContextDefaultLogLevel;
            Logger(std::string ctxId,
                   std::string ctxDescription,
                   LogLevel ctxDefLogLevel);

        public:
            Logger() = delete;
            ~Logger() noexcept = default;

            /// @brief Remote logging client connection state
            /// @returns Client connection state
            ClientState RemoteClientState() const noexcept;

            /// @brief Create a stream for fatal logs
            /// @returns Fatal log stream in the current context
            /// @see WithLevel
            LogStream LogFatal() const;

            /// @brief Create a stream for error logs
            /// @returns Error log stream in the current context
            /// @see WithLevel
            LogStream LogError() const;

            /// @brief Create a stream for warning logs
            /// @returns Warning log stream in the current context
            /// @see WithLevel
            LogStream LogWarn() const;

            /// @brief Create a stream for information logs
            /// @returns Information log stream in the current context
            /// @see WithLevel
            LogStream LogInfo() const;

            /// @brief Create a stream for debug logs
            /// @returns Debug log stream in the current context
            /// @see WithLevel
            LogStream LogDebug() const;

            /// @brief Create a stream for verbose logs
            /// @returns Verbose log stream in the current context
            /// @see WithLevel
            LogStream LogVerbose() const;

            /// @brief Determine whether a certian log level is enabled in the current context or not
            /// @param logLevel Input log severity level
            /// @returns True if the level is enabled; otherwise false
            bool IsEnabled(LogLevel logLevel) const noexcept;

            /// @brief Create a stream for certian level logs
            /// @param logLevel Input log severity level
            /// @returns Log stream with the determined level in the current context
            LogStream WithLevel(LogLevel logLevel) const;

            /// @brief Logger factory
            /// @param ctxId Context ID
            /// @param ctxDescription Context description
            /// @param ctxDefLogLevel Context default log level
            /// @returns A new logger for that specifc context
            /// @note Log with less severity than the default log level are ignored.
            static Logger CreateLogger(
                std::string ctxId,
                std::string ctxDescription,
                LogLevel ctxDefLogLevel);
        };
    }
}

#endif