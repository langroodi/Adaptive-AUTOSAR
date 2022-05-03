#include <gtest/gtest.h>
#include "../../../src/ara/diag/operation_cycle.h"

namespace ara
{
    namespace diag
    {
        class OperationCycleTest : public testing::Test
        {
        protected:
            bool OperationCycleRestarted;

        public:
            OperationCycleTest() : OperationCycleRestarted{false}
            {
            }

            void OnOperationCycleRestarted()
            {
                OperationCycleRestarted = true;
            }
        };

        TEST_F(OperationCycleTest, RestartScenario)
        {
            const ara::core::InstanceSpecifier cSpecifier("Instance0");
            OperationCycle _operationCycle(cSpecifier);

            auto _callback{
                std::bind(&OperationCycleTest::OnOperationCycleRestarted, this)};
            _operationCycle.SetNotifier(_callback);
            _operationCycle.RestartOperationCycle();

            EXPECT_TRUE(OperationCycleRestarted);
        }
    }
}