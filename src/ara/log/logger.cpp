#include "./logger.h"

namespace ara
{
    namespace log
    {
        Logger::Logger(std::string ctxId,
                       std::string ctxDescription,
                       LogLevel ctxDefLogLevel) : mContextId{ctxId},
                                                  mContextDescription{ctxDescription},
                                                  mContextDefaultLogLevel{ctxDefLogLevel}
        {
        }

        ClientState Logger::RemoteClientState() const noexcept
        {
            // For now, no client exists for logging modeled messages.
            return ClientState::kNotConnected;
        }

        LogStream Logger::LogFatal() const
        {
            LogStream _result = WithLevel(LogLevel::kFatal);
            return _result;
        }
        
        LogStream Logger::LogError() const
        {
            LogStream _result = WithLevel(LogLevel::kError);
            return _result;
        }

        LogStream Logger::LogWarn() const
        {
            LogStream _result = WithLevel(LogLevel::kWarn);
            return _result;
        }

        LogStream Logger::LogInfo() const
        {
            LogStream _result = WithLevel(LogLevel::kInfo);
            return _result;
        }

        LogStream Logger::LogDebug() const
        {
            LogStream _result = WithLevel(LogLevel::kDebug);
            return _result;
        }

        LogStream Logger::LogVerbose() const
        {
            LogStream _result = WithLevel(LogLevel::kVerbose);
            return _result;
        }

        bool Logger::IsEnabled(LogLevel logLevel) const noexcept
        {
            // Log levels are sorted in descending order
            bool _result = (logLevel <= mContextDefaultLogLevel);
            return _result;
        }

        LogStream Logger::WithLevel(LogLevel logLevel) const
        {
            const std::string cContextId{"Context ID:"};
            const std::string cContextDescription{"Context Description:"};
            const std::string cLogLevel{"Log Level:"};
            const std::string cSeperator{";"};

            LogStream _result;
            _result << cContextId << mContextId << cSeperator;
            _result << cContextDescription << mContextDescription << cSeperator;
            _result << cLogLevel << logLevel << cSeperator;

            return _result;
        }

        Logger Logger::CreateLogger(
            std::string ctxId,
            std::string ctxDescription,
            LogLevel ctxDefLogLevel)
        {
            Logger _result(ctxId, ctxDescription, ctxDefLogLevel);
            return _result;
        }
    }
}