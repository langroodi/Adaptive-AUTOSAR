#include <gtest/gtest.h>
#include <algorithm>
#include <array>
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

            TEST(LoadBalancingOptionTest, PayloadMethod)
            {
                const bool cDiscardable = false;
                const uint16_t cPriority = 1;
                const uint16_t cWeight = 2;
                const OptionType cType = OptionType::LoadBalancing;

                LoadBalancingOption _option(cDiscardable, cPriority, cWeight);

                const size_t cPayloadSize = 8;
                const std::array<uint8_t, cPayloadSize> cExpectedPayload =
                    {0x00, 0x05, 0x02, 0x00,
                     0x00, 0x01, 0x00, 0x02};

                auto _actualPayload = _option.Payload();

                bool _areEqual =
                    std::equal(
                        _actualPayload.begin(),
                        _actualPayload.end(),
                        cExpectedPayload.begin());

                EXPECT_TRUE(_areEqual);
            }
        }
    }
}