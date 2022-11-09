#include "./modelled_process.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            ModelledProcess::ModelledProcess() noexcept : mCancellationToken{false}
            {
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
            }
        }
    }
}