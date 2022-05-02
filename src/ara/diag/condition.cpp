#include "./condition.h"

namespace ara
{
    namespace diag
    {
        Condition::Condition(const ara::core::InstanceSpecifier &specifier) : mSpecifier{specifier},
                                                                              mCondition{ConditionType::kConditionFalse}
        {
        }

        ara::core::Result<ConditionType> Condition::GetCondition()
        {
            ara::core::Result<ConditionType> _result(mCondition);
            return _result;
        }

        ara::core::Result<void> Condition::SetCondition(ConditionType condition)
        {
            mCondition = condition;
            ara::core::Result<void> _result;

            return _result;
        }
    }
}