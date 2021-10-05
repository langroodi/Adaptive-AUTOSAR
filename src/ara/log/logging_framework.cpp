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

        const Logger &LoggingFramework::CreateLogger(
            std::string ctxId,
            std::string ctxDescription)
        {
            Logger _logger =
                Logger::CreateLogger(ctxId, ctxDescription, mDefaultLogLevel);
            mLoggers.push_back(std::move(_logger));
            const Logger &_result = mLoggers.back();

            return _result;
        }

        const Logger &LoggingFramework::CreateLogger(
            std::string ctxId,
            std::string ctxDescription,
            LogLevel ctxDefLogLevel)
        {
            {
                Logger _logger =
                    Logger::CreateLogger(ctxId, ctxDescription, ctxDefLogLevel);
                mLoggers.push_back(std::move(_logger));
                const Logger &_result = mLoggers.back();

                return _result;
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
                LogStream _logStreamContex = logger.WithLevel(logLevel);
                _logStreamContex << logStream;
                mLogSink->Log(_logStreamContex);
            }
        }

        LoggingFramework *LoggingFramework::Create(
            std::string appId,
            LogMode logMode,
            LogLevel logLevel,
            std::string appDescription)
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
                LoggingFramework *_result =
                    new LoggingFramework(_logSink, logLevel);

                return _result;
            }
            else
            {
                throw std::invalid_argument(
                    "The log mode is not currently supported.");
            }
        }

        LoggingFramework *LoggingFramework::Create(
            std::string appId,
            std::string filePath,
            LogLevel logLevel,
            std::string appDescription)
        {
            sink::LogSink *_logSink =
                new sink::FileLogSink(filePath, appId, appDescription);
            LoggingFramework *_result =
                new LoggingFramework(_logSink, logLevel);

            return _result;
        }

        LoggingFramework::~LoggingFramework() noexcept
        {
            delete mLogSink;
        }
    }
}