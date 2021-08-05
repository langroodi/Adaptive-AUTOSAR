#include <iostream>
#include <gtest/gtest.h>
#include "../../../src/ara/sm/states.h"
#include "../../../src/ara/sm/trigger_in.h"

namespace ara
{
    namespace sm
    {
        static void OnTriggeredHandler()
        {
            std::cout << "State changed." << std::endl;
        }

        TEST(SMTriggerInTest, TriggerWrite)
        {
            auto _initialState = FunctionGroupStates::kOff;

            TriggerIn<FunctionGroupStates> _triggerIn(_initialState, OnTriggeredHandler);

            auto _newState = FunctionGroupStates::kRunning;
            EXPECT_NO_THROW(_triggerIn.Trigger().Write(_newState));
        }
    }
}