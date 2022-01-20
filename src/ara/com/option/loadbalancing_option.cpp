#include "./loadbalancing_option.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            uint16_t LoadBalancingOption::Length() const noexcept
            {
                const uint8_t cOptionLength = 5;
                return cOptionLength;
            }

            uint16_t LoadBalancingOption::Priority() const noexcept
            {
                return mPriority;
            }

            uint16_t LoadBalancingOption::Weight() const noexcept
            {
                return mWeight;
            }

            std::vector<uint8_t> LoadBalancingOption::Payload() const
            {
                auto _result = Option::BasePayload();

                helper::Inject(_result, mPriority);
                helper::Inject(_result, mWeight);

                return _result;
            }

            std::unique_ptr<LoadBalancingOption> LoadBalancingOption::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset,
                bool discardable)
            {
                uint16_t _priority = helper::ExtractShort(payload, offset);
                uint16_t _weight = helper::ExtractShort(payload, offset);
                auto _result =
                    std::make_unique<LoadBalancingOption>(
                        discardable, _priority, _weight);

                return _result;
            }
        }
    }
}