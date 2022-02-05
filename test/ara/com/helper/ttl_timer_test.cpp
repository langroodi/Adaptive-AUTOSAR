#include <gtest/gtest.h>
#include "../../../../src/ara/com/helper/ttl_timer.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            TEST(TtlTimerTest, Constructor)
            {
                TtlTimer _timer;

                EXPECT_FALSE(_timer.GetRequested());
                EXPECT_FALSE(_timer.GetOffered());
            }

            TEST(TtlTimerTest, RequestedProperty)
            {
                const bool cExpectedResult{true};

                TtlTimer _timer;
                _timer.SetRequested(cExpectedResult);
                bool _actualResult = _timer.GetRequested();
                
                EXPECT_EQ(cExpectedResult, _actualResult);
            }

            TEST(TtlTimerTest, OfferedProperty)
            {
                const bool cTTL{1};

                TtlTimer _timer;
                _timer.SetOffered(cTTL);
                
                EXPECT_TRUE(_timer.GetOffered());
            }
        }
    }
}