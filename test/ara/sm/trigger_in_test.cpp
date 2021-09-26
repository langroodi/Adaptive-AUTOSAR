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
            auto _newState = FunctionGroupStates::kRunning;

            TriggerIn<FunctionGroupStates> _triggerIn(
                _initialState, OnTriggeredHandler);
            _triggerIn.GetTrigger().Write(_newState);

            EXPECT_EQ(_initialState, _newState);
        }
    }
}