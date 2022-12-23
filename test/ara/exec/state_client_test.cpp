#include <gtest/gtest.h>
#include "../../../src/ara/exec/state_client.h"
#include "./helper/mock_rpc_client.h"

namespace ara
{
    namespace exec
    {
        class StateClientTest : public testing::Test
        {
        private:
            helper::MockRpcClient mRpcClient;
            std::function<void(const ExecutionErrorEvent &)> mEmptyCallback;

        protected:
            const std::future_status cTimeoutStatus{std::future_status::timeout};
            const core::InstanceSpecifier cInstance{"test_instance"};
            const std::chrono::seconds cTimeout{30};

            StateClient Client;

            StateClientTest() : Client{mEmptyCallback, &mRpcClient}
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

        TEST_F(StateClientTest, SetStateMethod)
        {
            const std::string cState{"stateXYZ"};

            auto _functionGroup =
                FunctionGroup::Create(cInstance.ToString()).Value();
            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();

            std::shared_future<void> _succeed{Client.SetState(_functionGroupState)};
            EXPECT_TRUE(_succeed.valid());

            std::future_status _status{_succeed.wait_for(cTimeout)};
            EXPECT_NE(_status, cTimeoutStatus);
            EXPECT_NO_THROW(_succeed.get());
        }

        TEST_F(StateClientTest, GetInitialMachineStateTransitionResultMehod)
        {
            std::shared_future<void> _succeed{
                Client.GetInitialMachineStateTransitionResult()};

            std::future_status _status{_succeed.wait_for(cTimeout)};
            EXPECT_NE(_status, cTimeoutStatus);
            EXPECT_NO_THROW(_succeed.get());
        }

        TEST_F(StateClientTest, RedundantRequestScenario)
        {
            SetBypass(true);
            std::shared_future<void> _succeed{
                Client.GetInitialMachineStateTransitionResult()};

            SetBypass(false);
            std::shared_future<void> _redundantSucceed{
                Client.GetInitialMachineStateTransitionResult()};

            std::future_status _status{_succeed.wait_for(cTimeout)};
            EXPECT_NE(_status, cTimeoutStatus);
            EXPECT_THROW(_succeed.get(), ExecException);

            _status = _redundantSucceed.wait_for(cTimeout);
            EXPECT_NE(_status, cTimeoutStatus);
            EXPECT_NO_THROW(_redundantSucceed.get());
        }

        TEST_F(StateClientTest, TimeoutScenario)
        {
            const std::chrono::seconds _timeout(1);
            SetBypass(true);
            std::shared_future<void> _succeed{
                Client.GetInitialMachineStateTransitionResult()};

            std::future_status _status{_succeed.wait_for(_timeout)};
            EXPECT_EQ(_status, cTimeoutStatus);
        }

        TEST_F(StateClientTest, CorruptedResponseScenario)
        {
            SetRpcPayload(std::vector<uint8_t>({0x00}));
            std::shared_future<void> _succeed{
                Client.GetInitialMachineStateTransitionResult()};

            std::future_status _status{_succeed.wait_for(cTimeout)};
            EXPECT_NE(_status, cTimeoutStatus);
            EXPECT_THROW(_succeed.get(), ExecException);
        }

        TEST_F(StateClientTest, RequestFailureScenario)
        {
            SetRpcPayload(std::vector<uint8_t>({0, 0, 0, 1}));
            std::shared_future<void> _succeed{
                Client.GetInitialMachineStateTransitionResult()};

            std::future_status _status{_succeed.wait_for(cTimeout)};
            EXPECT_NE(_status, cTimeoutStatus);
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