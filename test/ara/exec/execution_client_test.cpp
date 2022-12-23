#include <gtest/gtest.h>
#include "../../../src/ara/exec/execution_client.h"
#include "./helper/mock_rpc_client.h"

namespace ara
{
    namespace exec
    {
        class ExecutionClientTest : public testing::Test
        {
        private:
            static const int64_t cTimeout{2};
            const core::InstanceSpecifier cInstance{"test_instance"};
            helper::MockRpcClient mRpcClient;

        protected:
            ExecutionClient Client;

            ExecutionClientTest() : Client{cInstance, &mRpcClient, cTimeout}
            {
            }

            void SetBypass(bool bypass)
            {
                mRpcClient.SetBypass(bypass);
            }

            void SetRpcPayload(std::vector<uint8_t> &&rpcPayload)
            {
                mRpcClient.SetRpcPayload(std::move(rpcPayload));
            }
        };

        const int64_t ExecutionClientTest::cTimeout;

        TEST_F(ExecutionClientTest, ReportExecutionStateMethod)
        {
            const ExecutionState cState{ExecutionState::kRunning};
            core::Result<void> _result{Client.ReportExecutionState(cState)};
            EXPECT_TRUE(_result.HasValue());
        }

        TEST_F(ExecutionClientTest, RedundantRequestScenario)
        {
            const ExecutionState cState{ExecutionState::kRunning};

            core::Result<void> _result{Client.ReportExecutionState(cState)};
            EXPECT_TRUE(_result.HasValue());

            core::Result<void> _redundantResult{Client.ReportExecutionState(cState)};
            EXPECT_TRUE(_redundantResult.HasValue());
        }

        TEST_F(ExecutionClientTest, TimeoutScenario)
        {
            const ExecErrc cExpectedResult{ExecErrc::kCommunicationError};
            const ExecutionState cState{ExecutionState::kRunning};

            SetBypass(true);
            core::Result<void> _result{Client.ReportExecutionState(cState)};

            EXPECT_FALSE(_result.HasValue());
            auto _actualResult{static_cast<ExecErrc>(_result.Error().Value())};
            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(ExecutionClientTest, CorruptedResponseScenario)
        {
            const ExecErrc cExpectedResult{ExecErrc::kCommunicationError};
            const ExecutionState cState{ExecutionState::kRunning};

            SetRpcPayload(std::vector<uint8_t>({0x00}));
            core::Result<void> _result{Client.ReportExecutionState(cState)};

            EXPECT_FALSE(_result.HasValue());
            auto _actualResult{static_cast<ExecErrc>(_result.Error().Value())};
            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(ExecutionClientTest, RequestFailureScenario)
        {
            const ExecErrc cExpectedResult{ExecErrc::kGeneralError};
            const ExecutionState cState{ExecutionState::kRunning};

            std::vector<uint8_t> _rcpPayload;
            auto _errorCodeInt{static_cast<uint32_t>(cExpectedResult)};
            com::helper::Inject(_rcpPayload, _errorCodeInt);
            SetRpcPayload(std::move(_rcpPayload));
            core::Result<void> _result{Client.ReportExecutionState(cState)};

            EXPECT_FALSE(_result.HasValue());
            auto _actualResult{static_cast<ExecErrc>(_result.Error().Value())};
            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}