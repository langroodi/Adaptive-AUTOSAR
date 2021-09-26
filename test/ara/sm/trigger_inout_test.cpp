#include <gtest/gtest.h>
#include "../../../src/ara/sm/states.h"
#include "../../../src/ara/sm/trigger_inout.h"

namespace ara
{
    namespace sm
    {
        class TriggerInOutTest : public testing::Test
        {
        protected:
            FunctionGroupStates CurrentState;
            TriggerInOut<FunctionGroupStates> TriggerIO;

            TriggerInOutTest() : CurrentState{FunctionGroupStates::kOff},
                                 TriggerIO(CurrentState, std::bind(&TriggerInOutTest::OnTriggeredHandler, this))
            {
                auto _handler =
                    std::bind(
                        &TriggerInOutTest::OnNotifiedHandler,
                        this,
                        std::placeholders::_1);
                TriggerIO.GetNotifier().Subscribe(_handler);
            }

            void OnTriggeredHandler()
            {
                TriggerIO.GetNotifier().Notify();
            }

            void OnNotifiedHandler(FunctionGroupStates newState)
            {
                EXPECT_EQ(CurrentState, newState);
            }
        };

        TEST_F(TriggerInOutTest, TriggerChain)
        {
            auto _newState = FunctionGroupStates::kRunning;
            EXPECT_NO_THROW(TriggerIO.GetTrigger().Write(_newState));
        }
    }
}