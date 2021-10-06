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
                TtlTimer _ttlTimer([&_expired]()
                                   { _expired = true; });

                _ttlTimer.Set(cTtl);
                _ttlTimer.Cancel();

                EXPECT_FALSE(_expired);
            }
        }
    }
}