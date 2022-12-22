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
            const core::InstanceSpecifier cInstance{"test_instance"};
            helper::MockRpcClient mRpcClient;

        protected:
            ExecutionClient Client;

            ExecutionClientTest() : Client{cInstance, &mRpcClient}
            {
            }
        };

        TEST_F(ExecutionClientTest, ReportExecutionStateMethod)
        {
            const ExecutionState cState{ExecutionState::kRunning};
            core::Result<void> _result{Client.ReportExecutionState(cState)};
            EXPECT_TRUE(_result.HasValue());
        }
    }
}