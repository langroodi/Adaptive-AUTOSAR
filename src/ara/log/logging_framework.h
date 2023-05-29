#ifndef LOGGING_FRAMEWORK_H
#define LOGGING_FRAMEWORK_H

#include <stdexcept>

#include "./logger.h"
#include "./sink/log_sink.h"
#include "./sink/console_log_sink.h"
#include "./sink/file_log_sink.h"

namespace ara
{
    namespace log
    {
        /// @brief Logging framework which links loggers to a log sink
        class LoggingFramework
        {
        private:
            sink::LogSink *mLogSink;
            LogLevel mDefaultLogLevel;
            std::vector<Logger> mLoggers;

            LoggingFramework(sink::LogSink *logSink, LogLevel logLevel);

        public:
            LoggingFramework() = delete;
            ~LoggingFramework() noexcept;

            /// @brief Create a logger
            /// @param ctxId Log context ID
            /// @param ctxDescription Log context description
            /// @returns A logger
            const Logger &CreateLogger(
                std::string ctxId,
                std::string ctxDescription);

            /// @brief Create a logger
            /// @param ctxId Log context ID
            /// @param ctxDescription Log context description
            /// @param ctxDefLogLevel Log context default log level
            /// @returns A logger
            const Logger &CreateLogger(
                std::string ctxId,
                std::string ctxDescription,
                LogLevel ctxDefLogLevel);

            /// @brief Log a stream to the determined sink
            /// @param logger A logger
            /// @param logLevel Log severity level
            /// @param logStream Stream to be logged
            void Log(
                const Logger &logger,
                LogLevel logLevel,
                const LogStream &logStream);

            /// @brief Logging framework factory
            /// @param appId Application ID
            /// @param logMode Log sink mode
            /// @param logLevel Log severity level
            /// @param appDescription Application description
            /// @returns Pointer to created logging framework
            /// @throws std::invalid_argument Throws when file log mode is chosen
            /// @note To create a framework to use a file sink refer to see aslo
            /// @see Create(std::string, std::string, LogLevel, std::string)
            static LoggingFramework *Create(
                std::string appId,
                LogMode logMode,
                LogLevel logLevel = LogLevel::kWarn,
                std::string appDescription = "");

            /// @brief Logging framework factory only for file sinks
            /// @param appId Application ID
            /// @param filePath Log file path
            /// @param logLevel Log severity level
            /// @param appDescription Application description
            /// @returns Pointer to created logging framework
            /// @note To create a framework for other sinks refer to see also
            /// @see Create(std::string, LogMode, LogLevel, std::string)
            static LoggingFramework *Create(
                std::string appId,
                std::string filePath,
                LogLevel logLevel = LogLevel::kWarn,
                std::string appDescription = "");
        };
    }
}

#endif