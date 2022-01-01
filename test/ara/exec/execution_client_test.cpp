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
            using FifoMessageType = std::pair<core::InstanceSpecifier, ExecutionState>;

            const core::InstanceSpecifier cInstance{"test_instance"};
            helper::MockupFifoLayer<FifoMessageType> mCommunicationLayer;

            void receiver(FifoMessageType message)
            {
                if (message.first == cInstance)
                {
                    ReportedState = message.second;
                }
            }

        protected:
            ExecutionClient Client;
            core::Optional<ExecutionState> ReportedState;

            ExecutionClientTest() : Client{cInstance, &mCommunicationLayer}
            {
                auto _receiver =
                    std::bind(
                        &ExecutionClientTest::receiver, this, std::placeholders::_1);

                mCommunicationLayer.SetReceiver(_receiver);
            }
        };

        TEST_F(ExecutionClientTest, ReportExecutionStateMethod)
        {
            const ExecutionState cState{ExecutionState::kRunning};
            EXPECT_FALSE(ReportedState.HasValue());

            Client.ReportExecutionState(cState);

            EXPECT_TRUE(ReportedState.HasValue());
            EXPECT_EQ(cState, ReportedState.Value());
        }
    }
}