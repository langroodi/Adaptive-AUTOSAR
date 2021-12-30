#include <gtest/gtest.h>
#include "../../../src/ara/exec/function_group_state.h"

namespace ara
{
    namespace exec
    {
        TEST(FunctionGroupStateTest, Factory)
        {
            const std::string cShortnamePath{"application"};
            const std::string cState{"stateXYZ"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();

            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();

            EXPECT_TRUE(_functionGroupState.GetFunctionGroup() == _functionGroup);
            EXPECT_TRUE(_functionGroupState.GetState() == cState);
        }

        TEST(FunctionGroupStateTest, MoveConstructor)
        {
            const std::string cShortnamePath{"application"};
            const std::string cState{"stateXYZ"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();

            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();
            FunctionGroupState _otherFunctionGroupState{std::move(_functionGroupState)};

            EXPECT_TRUE(_otherFunctionGroupState.GetFunctionGroup() == _functionGroup);
            EXPECT_TRUE(_otherFunctionGroupState.GetState() == cState);
        }

        TEST(FunctionGroupStateTest, MoveAssignment)
        {
            const std::string cShortnamePath{"application"};
            const std::string cState{"stateXYZ"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();

            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();
            FunctionGroupState _otherFunctionGroupState = std::move(_functionGroupState);

            EXPECT_TRUE(_otherFunctionGroupState.GetFunctionGroup() == _functionGroup);
            EXPECT_TRUE(_otherFunctionGroupState.GetState() == cState);
        }

        TEST(FunctionGroupStateTest, Equality)
        {
            const std::string cShortnamePath{"application"};
            const std::string cState{"stateXYZ"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();

            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();
            auto _otherFunctionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();

            EXPECT_TRUE(_functionGroupState == _otherFunctionGroupState);
        }

        TEST(FunctionGroupStateTest, Inequality)
        {
            const std::string cShortnamePath{"application"};
            const std::string cState{"stateXYZ"};
            const std::string cOtherState{"stateOtherXYZ"};
            auto _functionGroup = FunctionGroup::Create(cShortnamePath).Value();

            auto _functionGroupState =
                FunctionGroupState::Create(_functionGroup, cState).Value();
            auto _otherFunctionGroupState =
                FunctionGroupState::Create(_functionGroup, cOtherState).Value();

            EXPECT_TRUE(_functionGroupState != _otherFunctionGroupState);
        }
    }
}