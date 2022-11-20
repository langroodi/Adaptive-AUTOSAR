#include "./state_management.h"

namespace application
{
    namespace platform
    {
        const std::string StateManagement::cAppId{"StateManagement"};
        const ara::log::LogMode StateManagement::cLogMode{ara::log::LogMode::kConsole};
        const std::string StateManagement::cContextId{"Lifetime"};
        const std::string StateManagement::cContextDescription{"Application lifetime logs"};
        const ara::log::LogLevel StateManagement::cLogLevel{ara::log::LogLevel::kInfo};

        StateManagement::StateManagement() : mLoggingFramework{ara::log::LoggingFramework::Create(cAppId, cLogMode)},
                                             mLogger{mLoggingFramework->CreateLogger(cContextId, cContextDescription, cLogLevel)}
        {
        }

        int StateManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const int cSuccessfulExitCode{0};
            ara::log::LogStream _logStream;

            _logStream << "State management has been initialized.";
            mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

            while (!cancellationToken->load())
            {
                mDeterministicClient.WaitForActivation();

                ///@todo State management logic here
            }

            _logStream.Flush();
            _logStream << "State management has been terminated.";
            mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

            return cSuccessfulExitCode;
        }

        StateManagement::~StateManagement()
        {
            delete mLoggingFramework;
        }
    }
}