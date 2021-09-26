#include <iostream>
#include <gtest/gtest.h>
#include "../../../src/ara/sm/states.h"
#include "../../../src/ara/sm/trigger_out.h"

namespace ara
{
    namespace sm
    {
        static void OnNotifiedHandler(FunctionGroupStates newState)
        {
            std::cout << "Notification received." << std::endl;
        }

        TEST(SMTriggerOutTest, NotifierSubscribeAndNotify)
        {
            auto _initialState = FunctionGroupStates::kOff;
            TriggerOut<FunctionGroupStates> _triggerOut(_initialState);
            _triggerOut.GetNotifier().Subscribe(OnNotifiedHandler);
            _initialState = FunctionGroupStates::kRunning;

            EXPECT_NO_THROW(_triggerOut.GetNotifier().Notify());
        }
    }
}