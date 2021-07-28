#include <stdexcept>
#include <iostream>
#include "./logger.h"
#include "./sink/log_sink.h"
#include "./sink/console_log_sink.h"
#include "./sink/file_log_sink.h"

namespace ara
{
    namespace log
    {
        class LoggingFramework
        {
        private:
            sink::LogSink* mLogSink;
            LogLevel mDefaultLogLevel;
            std::vector<Logger> mLoggers;

            LoggingFramework(sink::LogSink* logSink, LogLevel logLevel);

        public:
            LoggingFramework() = default;
            ~LoggingFramework() noexcept;
            Logger &CreateLogger(
                std::string ctxId,
                std::string ctxDescription);
            Logger &CreateLogger(
                std::string ctxId,
                std::string ctxDescription,
                LogLevel ctxDefLogLevel);
            void Log(
                const Logger& logger,
                LogLevel logLevel,
                const LogStream& logStream);

           static LoggingFramework Create(
                std::string appId,
                LogMode logMode,
                LogLevel logLevel = LogLevel::kWarn,
                std::string appDescription = "");
            static LoggingFramework Create(
                std::string appId,
                std::string filePath,
                LogLevel logLevel = LogLevel::kWarn,
                std::string appDescription = "");                
        };
    }
}