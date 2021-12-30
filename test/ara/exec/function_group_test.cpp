#include <gtest/gtest.h>
#include "../../../src/ara/exec/function_group.h"

namespace ara
{
    namespace exec
    {
        TEST(FunctionGroupTest, Factory)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();

            EXPECT_TRUE(_functionGroup.GetInstance() == cShortnamePath);
        }

        TEST(FunctionGroupTest, MoveConstructor)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();
            auto _otherFunctionGroup{std::move(_functionGroup)};

            EXPECT_TRUE(_otherFunctionGroup.GetInstance() == cShortnamePath);
        }

        TEST(FunctionGroupTest, MoveAssignment)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();
            auto _otherFunctionGroup = std::move(_functionGroup);

            EXPECT_TRUE(_otherFunctionGroup.GetInstance() == cShortnamePath);
        }

        TEST(FunctionGroupTest, Equality)
        {
            const std::string cShortnamePath{"application"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();
            auto _otherFunctionGroup = FunctionGroup::Create(cShortnamePath).Value();

            EXPECT_TRUE(_functionGroup == _otherFunctionGroup);
        }

        TEST(FunctionGroupTest, Inequality)
        {
            const std::string cShortnamePath{"application"};
            const std::string cOtherShortnamePath{"otherApplication"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();
            auto _otherFunctionGroup = FunctionGroup::Create(cOtherShortnamePath).Value();

            EXPECT_TRUE(_functionGroup != _otherFunctionGroup);
        }
    }
}