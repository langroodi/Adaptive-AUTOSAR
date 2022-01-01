#include <gtest/gtest.h>
#include "../../../src/ara/exec/state_client.h"
#include "./helper/mockup_fifo_layer.h"

namespace ara
{
    namespace exec
    {
        class StateClientTest : public testing::Test
        {
        private:
            using SetStateMessage = std::pair<const FunctionGroup *, std::string>;

            helper::MockupFifoLayer<SetStateMessage> mCommunicationLayer;
            std::function<void(const ExecutionErrorEvent &)> mEmptyCallback;

            void receiver(SetStateMessage message)
            {
                if (message.first->GetInstance() == cInstance)
                {
                    RequestedState = message.second;
                }
            }

        protected:
            const core::InstanceSpecifier cInstance{"test_instance"};
            StateClient Client;
            std::string RequestedState;

            StateClientTest() : Client{mEmptyCallback, &mCommunicationLayer}
            {
                auto _receiver =
                    std::bind(
                        &StateClientTest::receiver, this, std::placeholders::_1);

                mCommunicationLayer.SetReceiver(_receiver);
            }
        };

        TEST_F(StateClientTest, SetStateMethod)
        {
            const std::string cState{"stateXYZ"};

            auto _functionGroup =
                FunctionGroup::Create(cInstance.ToString()).Value();
            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();

            Client.SetState(_functionGroupState);

            EXPECT_EQ(cState, RequestedState);
        }

        TEST_F(StateClientTest, GetInitialMachineStateTransitionResultMehod)
        {
            std::future<void> _succeed =
                Client.GetInitialMachineStateTransitionResult();
            EXPECT_TRUE(_succeed.valid());

            _succeed.wait();
            EXPECT_NO_THROW(_succeed.get());

            _succeed = Client.GetInitialMachineStateTransitionResult();
            EXPECT_TRUE(_succeed.valid());

            _succeed.wait();
            EXPECT_THROW(_succeed.get(), ExecException);
        }

        TEST_F(StateClientTest, GetExecutionErrorMethod)
        {
            const ExecErrc cExpectedResult{ExecErrc::kFailed};

            auto _functionGroup =
                FunctionGroup::Create(cInstance.ToString()).Value();

            core::ErrorCode _errorCode =
                Client.GetExecutionError(_functionGroup).Error();

            auto _actualResult = static_cast<ExecErrc>(_errorCode.Value());

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}