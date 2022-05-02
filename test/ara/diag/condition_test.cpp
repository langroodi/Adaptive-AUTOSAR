#include <gtest/gtest.h>
#include "../../../src/ara/diag/condition.h"

namespace ara
{
    namespace diag
    {
        TEST(ConditionTest, ConditionProperty)
        {
            const ara::core::InstanceSpecifier cSpecifier("Instance0");
            const ConditionType cExpectedResult{ConditionType::kConditionTrue};
            Condition _condition(cSpecifier);
            auto _result{_condition.SetCondition(cExpectedResult)};
            EXPECT_TRUE(_result.HasValue());
            auto _currentCoditionStatus{_condition.GetCondition()};
            EXPECT_TRUE(_currentCoditionStatus.HasValue());
            ConditionType _actualResult{_currentCoditionStatus.Value()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}