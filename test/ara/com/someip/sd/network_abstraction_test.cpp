#include <gtest/gtest.h>
#include <asyncbsdsocket/poller.h>

using namespace AsyncBsdSocketLib;

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                TEST(NetworkAbstractionTest, PollerConstructor)
                {
                    EXPECT_NO_THROW(Poller poller;);
                }
            }
        }
    }
}