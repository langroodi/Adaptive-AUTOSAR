#include <gtest/gtest.h>
#include "../../../src/ara/phm/recovery_action.h"

namespace ara
{
    namespace phm
    {
        static const core::InstanceSpecifier cSpecifier("Instance0");

        class RecoveryActionTest : public RecoveryAction, public testing::Test
        {
        public:
            RecoveryActionTest() : RecoveryAction(cSpecifier)
            {
            }

            void RecoveryHandler(
                const exec::ExecutionErrorEvent &executionError,
                TypeOfSupervision supervision) override
            {
                // Empty body
            }
        };

        TEST_F(RecoveryActionTest, Constructor)
        {
            EXPECT_FALSE(IsOffered());
        }

        TEST_F(RecoveryActionTest, OfferMethod)
        {
            const auto cResult{Offer()};
            EXPECT_TRUE(cResult.HasValue());
            EXPECT_TRUE(IsOffered());
        }

        TEST_F(RecoveryActionTest, StopOfferMethod)
        {
            Offer();
            StopOffer();
            EXPECT_FALSE(IsOffered());
        }
    }
}