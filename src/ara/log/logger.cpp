#include "./logger.h"

namespace ara
{
    namespace log
    {
        ClientState Logger::RemoteClientState() const noexcept
        {
            // For now, no client exists for logging modeled messages.
            return ClientState::kNotConnected;
        }

        bool Logger::IsEnabled(LogLevel logLevel) const noexcept
        {
            bool _result = logLevel <= mContextDefaultLogLevel;
            return _result;
        }
    }
}