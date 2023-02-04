#include "./modelled_process.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            const log::LogMode ModelledProcess::cLogMode{log::LogMode::kConsole};
            const std::string ModelledProcess::cContextId{"Lifetime"};
            const std::string ModelledProcess::cContextDescription{"Application lifetime logs"};
            const log::LogLevel ModelledProcess::cLogLevel{log::LogLevel::kInfo};
            const log::LogLevel ModelledProcess::cErrorLevel{log::LogLevel::kError};

            ModelledProcess::ModelledProcess(
                std::string appId, AsyncBsdSocketLib::Poller *poller) : Poller{poller},
                                                                        mLoggingFramework{log::LoggingFramework::Create(appId, cLogMode)},
                                                                        mLogger{mLoggingFramework->CreateLogger(cContextId, cContextDescription, cLogLevel)},
                                                                        mCancellationToken{false}
            {
            }

            void ModelledProcess::Log(
                log::LogLevel logLevel, const log::LogStream &logStream)
            {
                mLoggingFramework->Log(mLogger, logLevel, logStream);
            }

            bool ModelledProcess::WaitForActivation()
            {
                auto cActivationReturnResult{
                    mDeterministicClient.WaitForActivation()};
                const exec::ActivationReturnType cActivationReturn{
                    cActivationReturnResult.Value()};

                return cActivationReturn != exec::ActivationReturnType::kTerminate;
            }

            void ModelledProcess::Initialize(
                const std::map<std::string, std::string> &arguments)
            {
                // Invalid exit code refers to a not-started or terminated process
                if (!mExitCode.valid())
                {
                    mExitCode =
                        std::async(
                            std::launch::async,
                            &ModelledProcess::Main,
                            this, &mCancellationToken, arguments);
                }
            }

            int ModelledProcess::Terminate()
            {
                const int cSuccessfulExitCode{0};
                int _result;

                if (mExitCode.valid())
                {
                    // Set the cancellation token and wait for the exit code
                    mCancellationToken = true;
                    _result = mExitCode.get();
                }
                else
                {
                    // Exit with the successful code if the process has not been started
                    // or it has been already disposed after the termination
                    _result = cSuccessfulExitCode;
                }

                return _result;
            }

            ModelledProcess::~ModelledProcess()
            {
                Terminate();
                delete mLoggingFramework;
            }
        }
    }
}