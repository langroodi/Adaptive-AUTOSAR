#include <gtest/gtest.h>
#include "../../../../src/ara/com/option/loadbalancing_option.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            TEST(LoadBalancingOptionTest, Constructor)
            {
                const bool cDiscardable = true;
                const uint16_t cPriority = 1;
                const uint16_t cWeight = 2;
                const OptionType cType = OptionType::LoadBalancing;

                LoadBalancingOption _option(cDiscardable, cPriority, cWeight);

                EXPECT_EQ(_option.Discardable(), cDiscardable);
                EXPECT_EQ(_option.Priority(), cPriority);
                EXPECT_EQ(_option.Weight(), cWeight);
                EXPECT_EQ(_option.Type(), cType);
            }
        }
    }
}