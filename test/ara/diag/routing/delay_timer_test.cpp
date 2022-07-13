#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/delay_timer.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            TEST(DelayTimerTest, Constructor)
            {
                DelayTimer _delayTimer;
                EXPECT_FALSE(_delayTimer.IsActive());
            }

            TEST(DelayTimerTest, StartMethod)
            {
                const std::chrono::seconds cDelayDuration(1);
                const std::chrono::milliseconds cExpectActiveTime(100);
                const std::chrono::milliseconds cExpectInactiveTime(1000);

                DelayTimer _delayTimer;
                _delayTimer.Start(cDelayDuration);
                std::this_thread::sleep_for(cExpectActiveTime);
                EXPECT_TRUE(_delayTimer.IsActive());
                std::this_thread::sleep_for(cExpectInactiveTime);
                EXPECT_FALSE(_delayTimer.IsActive());
            }

            TEST(DelayTimerTest, DisposeMethod)
            {
                const std::chrono::seconds cDelayDuration(1);
                const std::chrono::milliseconds cExpectActiveTime(100);

                DelayTimer _delayTimer;
                _delayTimer.Start(cDelayDuration);
                std::this_thread::sleep_for(cExpectActiveTime);
                _delayTimer.Dispose();
                EXPECT_FALSE(_delayTimer.IsActive());
            }
        }
    }
}