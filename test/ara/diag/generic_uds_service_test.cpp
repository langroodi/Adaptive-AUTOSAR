#include <gtest/gtest.h>
#include "../../../src/ara/diag/diag_error_domain.h"
#include "../../../src/ara/diag/generic_uds_service.h"

namespace ara
{
    namespace diag
    {
        static const ara::core::InstanceSpecifier cSpecifier("Instance0");

        class GenericUdsServiceTest : public testing::Test, public GenericUDSService
        {
        private:
            static const ReentrancyType cReentrancy{ReentrancyType::kNot};
            static const uint8_t cSid{0x22};

        public:
            GenericUdsServiceTest() : GenericUDSService(cSpecifier, cReentrancy, cSid)
            {
            }

            std::future<OperationOutput> HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override
            {
                std::future<OperationOutput> _result;
                return _result;
            }
        };

        const ReentrancyType GenericUdsServiceTest::cReentrancy;

        TEST_F(GenericUdsServiceTest, OfferScenario)
        {
            auto _result{Offer()};

            EXPECT_TRUE(_result.HasValue());
            EXPECT_TRUE(IsOffered());
        }

        TEST_F(GenericUdsServiceTest, ReOfferScenario)
        {
            const DiagErrc cExpectedResult{DiagErrc::kAlreadyOffered};
            Offer();
            // Re-offer
            auto _result{Offer()};

            EXPECT_TRUE(IsOffered());
            EXPECT_FALSE(_result.HasValue());
            auto _actualResult{static_cast<DiagErrc>(_result.Error().Value())};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(GenericUdsServiceTest, StopOfferScenario)
        {
            Offer();
            StopOffer();

            EXPECT_FALSE(IsOffered());
        }
    }
}