#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include "../../../../src/ara/com/option/option_deserializer.h"

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

            TEST(LoadBalancingOptionTest, Deserializing)
            {
                const bool cDiscardable = false;
                const uint16_t cPriority = 1;
                const uint16_t cWeight = 2;

                LoadBalancingOption _originalOption(
                    cDiscardable, cPriority, cWeight);

                auto _payload = _originalOption.Payload();
                std::size_t _offset = 0;
                auto _deserializedOptionBase =
                    OptionDeserializer::Deserialize(_payload, _offset);

                auto _deserializedOption =
                    dynamic_cast<LoadBalancingOption *>(
                        _deserializedOptionBase.get());

                EXPECT_EQ(
                    _originalOption.Type(),
                    _deserializedOption->Type());

                EXPECT_EQ(
                    _originalOption.Discardable(),
                    _deserializedOption->Discardable());

                EXPECT_EQ(
                    _originalOption.Priority(),
                    _deserializedOption->Priority());

                EXPECT_EQ(
                    _originalOption.Weight(),
                    _deserializedOption->Weight());
            }
        }
    }
}