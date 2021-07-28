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
            ~Logger() noexcept = default;
            ClientState RemoteClientState() const noexcept;
            LogStream LogFatal() const;
            LogStream LogError() const;
            LogStream LogWarn() const;
            LogStream LogInfo() const;
            LogStream LogDebug() const;
            LogStream LogVerbose() const;
            bool IsEnabled(LogLevel logLevel) const noexcept;
            LogStream WithLevel(LogLevel logLevel) const;

            static Logger CreateLogger(
                std::string ctxId,
                std::string ctxDescription,
                LogLevel ctxDefLogLevel);
        };
    }
}

#endif