#include <gtest/gtest.h>
#include "../../../../src/ara/com/helper/ttl_timer.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            TEST(TtlTimerTest, Expiration)
            {
                const int cTtl = 1;

                bool _expired = false;
                TtlTimer _ttlTimer;

                _ttlTimer.SetExpirationCallback([&_expired]()
                                                { _expired = true; });
                _ttlTimer.Set(cTtl);
                _ttlTimer.Cancel();

                EXPECT_FALSE(_expired);
            }

            TEST(TtlTimerTest, Set)
            {
                const int cTtl = 1;

                bool _expired = false;
                TtlTimer _ttlTimer;

                _ttlTimer.Set(cTtl);
                // Timer has been already set which makes the following call invalid.
                EXPECT_THROW(_ttlTimer.Set(cTtl), std::logic_error);
                // Cancel the timer gracefully
                _ttlTimer.Cancel();
            }
        }
    }
}