#include "./loadbalancing_option.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            LoadBalancingOption::LoadBalancingOption(
                bool discardable,
                uint16_t priority,
                uint16_t weight) noexcept : Option(OptionType::LoadBalancing, discardable),
                                            mPriority{priority},
                                            mWeight{weight}
            {
            }

            uint16_t LoadBalancingOption::Length()
            {
                const uint8_t cOptionLength = 5;
                return cOptionLength;
            }

            uint16_t LoadBalancingOption::Priority()
            {
                return mPriority;
            }

            uint16_t LoadBalancingOption::Weight()
            {
                return mWeight;
            }

            std::vector<uint8_t> LoadBalancingOption::Payload()
            {
                auto _result = Option::Payload();

                helper::Inject(_result, mPriority);
                helper::Inject(_result, mWeight);

                return _result;
            }
        }
    }
}