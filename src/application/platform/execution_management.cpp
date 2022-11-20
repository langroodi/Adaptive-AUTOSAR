#include "./execution_management.h"

namespace application
{
    namespace platform
    {
        const std::string ExecutionManagement::cAppId{"ExecutionManagement"};
        const ara::log::LogMode ExecutionManagement::cLogMode{ara::log::LogMode::kConsole};
        const std::string ExecutionManagement::cContextId{"Lifetime"};
        const std::string ExecutionManagement::cContextDescription{"Application lifetime logs"};
        const ara::log::LogLevel ExecutionManagement::cLogLevel{ara::log::LogLevel::kInfo};

        ExecutionManagement::ExecutionManagement() : mLoggingFramework{ara::log::LoggingFramework::Create(cAppId, cLogMode)},
                                                     mLogger{mLoggingFramework->CreateLogger(cContextId, cContextDescription, cLogLevel)}
        {
        }

        int ExecutionManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            ara::log::LogStream _logStream;

            _logStream << "Execution management has been initialized.";
            mLoggingFramework->Log(mLogger, cLogLevel, _logStream);
            
            mStateManagement.Initialize(arguments);

            while (!cancellationToken->load())
            {
                mDeterministicClient.WaitForActivation();

                ///@todo Execution management logic here
            }

            int _result{mStateManagement.Terminate()};

            _logStream.Flush();
            _logStream << "Execution management has been terminated.";
            mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

            return _result;
        }

        ExecutionManagement::~ExecutionManagement()
        {
            mStateManagement.Terminate();
            delete mLoggingFramework;
        }
    }
}