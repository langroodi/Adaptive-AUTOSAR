#include "./logging_framework.h"

namespace ara
{
    namespace log
    {
        LoggingFramework::LoggingFramework(
            sink::LogSink *logSink,
            LogLevel logLevel) : mLogSink{logSink},
                                 mDefaultLogLevel{logLevel}
        {
        }

        Logger &LoggingFramework::CreateLogger(
            std::string ctxId,
            std::string ctxDescription)
        {
            Logger _logger =
                Logger::CreateLogger(ctxId, ctxDescription, mDefaultLogLevel);
            mLoggers.push_back(_logger);

            return _logger;
        }

        Logger &LoggingFramework::CreateLogger(
            std::string ctxId,
            std::string ctxDescription,
            LogLevel ctxDefLogLevel)
        {
            {
                Logger _logger =
                    Logger::CreateLogger(ctxId, ctxDescription, ctxDefLogLevel);
                mLoggers.push_back(_logger);

                return _logger;
            }
        }

        void LoggingFramework::Log(
            const Logger &logger,
            LogLevel logLevel,
            const LogStream &logStream)
        {
            bool _isLevelEnabled = logger.IsEnabled(logLevel);

            if (_isLevelEnabled)
            {
                mLogSink->Log(logStream);
            }
        }

        LoggingFramework LoggingFramework::Create(
            std::string appId,
            LogMode logMode,
            LogLevel logLevel = LogLevel::kWarn,
            std::string appDescription = "")
        {
            if (logMode == LogMode::kFile)
            {
                throw std::invalid_argument(
                    "File logging mode is not feasible within this constructor override.");
            }

            if (logMode == LogMode::kConsole)
            {
                sink::LogSink *_logSink =
                    new sink::ConsoleLogSink(appId, appDescription);
                LoggingFramework _result(_logSink, logLevel);
            }
            else
            {
                throw std::invalid_argument(
                    "The log mode is not currently supported.");
            }
        }

        LoggingFramework LoggingFramework::Create(
            std::string appId,
            std::string filePath,
            LogLevel logLevel = LogLevel::kWarn,
            std::string appDescription = "")
        {
            sink::LogSink *_logSink =
                new sink::FileLogSink(filePath, appId, appDescription);
            LoggingFramework _result(_logSink, logLevel);
        }

        LoggingFramework::~LoggingFramework() noexcept
        {
            delete mLogSink;
        }
    }
}