#ifndef LOGGER_H
#define LOGGER_H

#include "./log_stream.h"

namespace ara
{
    namespace log
    {
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
            ~Logger() noexcept;
            ClientState RemoteClientState() const noexcept;
            LogStream LogFatal() const noexcept;
            LogStream LogError() const noexcept;
            LogStream LogWarn() const noexcept;
            LogStream LogInfo() const noexcept;
            LogStream LogDebug() const noexcept;
            LogStream LogVerbose() const noexcept;
            bool IsEnabled(LogLevel logLevel) const noexcept;
            LogStream WithLevel(LogLevel logLevel) const noexcept;

            static Logger CreateLogger(
                std::string ctxId,
                std::string ctxDescription,
                LogLevel ctxDefLogLevel);
        };
    }
}

#endif