#include <thread>
#include <gtest/gtest.h>
#include "../../../../src/ara/exec/helper/modelled_process.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            class ModelledProcessTest : public ModelledProcess, public testing::Test
            {
            private:
                AsyncBsdSocketLib::Poller mPoller;

            protected:
                const int cExitCode{0};

                int Main(
                    const std::atomic_bool *cancellationToken,
                    const std::map<std::string, std::string> &arguments) override
                {
                    const std::chrono::milliseconds cSleepTime{1};

                    while (!cancellationToken->load())
                    {
                        std::this_thread::sleep_for(cSleepTime);
                    }

                    return cExitCode;
                }

            public:
                ModelledProcessTest() : ModelledProcess("TestApp", &mPoller)
                {
                }
            };

            TEST_F(ModelledProcessTest, InitializeMethod)
            {
                std::map<std::string, std::string> _arguments;
                Initialize(_arguments);
                int _actualResult{Terminate()};

                EXPECT_EQ(cExitCode, _actualResult);
            }

            TEST_F(ModelledProcessTest, TerminateMethod)
            {
                const int cExpectedResult{0};
                int _actualResult{Terminate()};

                EXPECT_EQ(cExpectedResult, _actualResult);
            }
        }
    }
}