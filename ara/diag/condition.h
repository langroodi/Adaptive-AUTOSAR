#ifndef CONDITION_H
#define CONDITION_H

#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        enum class ConditionType
        {
            kConditionFalse = 0x00,
            kConditionTrue = 0x01
        };

        class Condition
        {
        public:
            explicit Condition(const ara::core::InstanceSpecifier &specifier);
            ~Condition() noexcept = default;
            ara::core::Result<ConditionType> GetCondition();
            ara::core::Result<void> SetCondition(ConditionType condition);
        };
    }
}

#endif