#include <gtest/gtest.h>
#include "../../../src/ara/exec/function_group.h"

namespace ara
{
    namespace exec
    {
        TEST(FunctionGroupTest, Factory)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath);

            EXPECT_TRUE(_functionGroup.Value().GetInstance() == cShortnamePath);
        }

        TEST(FunctionGroupTest, MoveConstructor)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath);
            auto _otherFunctionGroup{std::move(_functionGroup)};

            EXPECT_TRUE(_otherFunctionGroup.Value().GetInstance() == cShortnamePath);
        }

        TEST(FunctionGroupTest, MoveAssignment)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath);
            auto _otherFunctionGroup = std::move(_functionGroup);

            EXPECT_TRUE(_otherFunctionGroup.Value().GetInstance() == cShortnamePath);
        }

        TEST(FunctionGroupTest, Equality)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath);
            auto _otherFunctionGroup = FunctionGroup::Create(cShortnamePath);

            EXPECT_TRUE(_functionGroup.Value() == _otherFunctionGroup.Value());
        }

        TEST(FunctionGroupTest, Inqquality)
        {
            const std::string cShortnamePath{"application"};
            const std::string cOtherShortnamePath{"otherApplication"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath);
            auto _otherFunctionGroup = FunctionGroup::Create(cOtherShortnamePath);

            EXPECT_TRUE(_functionGroup.Value() != _otherFunctionGroup.Value());
        }
    }
}