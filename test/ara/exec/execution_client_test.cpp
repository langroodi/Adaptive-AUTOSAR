#include <gtest/gtest.h>
#include "../../../src/ara/exec/execution_client.h"
#include "./helper/mockup_fifo_layer.h"

namespace ara
{
    namespace exec
    {
        class ExecutionClientTest : public testing::Test
        {
        private:
            const core::InstanceSpecifier cInstance{"test_instance"};
            helper::MockupFifoLayer<ExecutionClient::FifoMessageType> mCommunicationLayer;
            ExecutionClient mClient;

            void receiver(ExecutionClient::FifoMessageType message)
            {
                if (message.first == cInstance)
                {
                    ReportedState = message.second;
                }
            }

        protected:
            core::Optional<ExecutionState> ReportedState;

            ExecutionClientTest() : mClient{cInstance, &mCommunicationLayer}
            {
                auto _receiver =
                    std::bind(
                        &ExecutionClientTest::receiver, this, std::placeholders::_1);

                mCommunicationLayer.SetReceiver(_receiver);
            }

            void ReportExecutionState(ExecutionState state)
            {
                mClient.ReportExecutionState(state);
            }
        };

        TEST_F(ExecutionClientTest, ReportExecutionStateMethod)
        {
            const ExecutionState cState{ExecutionState::kRunning};
            EXPECT_FALSE(ReportedState.HasValue());

            ReportExecutionState(cState);

            EXPECT_TRUE(ReportedState.HasValue());
            EXPECT_EQ(cState, ReportedState.Value());
        }
    }
}