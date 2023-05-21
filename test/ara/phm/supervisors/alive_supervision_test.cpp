#include <gtest/gtest.h>
#include "../../../../src/ara/phm/supervisors/alive_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            TEST(AliveSupervisionTest, ZeroMinMarginConstruction)
            {
                const uint16_t cExpectedAliveIndications{2};
                const uint16_t cMinMargin{0};
                const uint16_t cMaxMargin{1};
                const std::chrono::milliseconds cAliveReferenceCycle{1};
                const uint8_t cFailedReferenceCyclesTolerance{1};

                EXPECT_THROW(
                    AliveSupervision _supervision(
                        cExpectedAliveIndications,
                        cMinMargin,
                        cMaxMargin,
                        cAliveReferenceCycle,
                        cFailedReferenceCyclesTolerance),
                    std::invalid_argument);
            }

            TEST(AliveSupervisionTest, BigMinMarginConstruction)
            {
                const uint16_t cExpectedAliveIndications{2};
                const uint16_t cMinMargin{2};
                const uint16_t cMaxMargin{1};
                const std::chrono::milliseconds cAliveReferenceCycle{1};
                const uint8_t cFailedReferenceCyclesTolerance{1};

                EXPECT_THROW(
                    AliveSupervision _supervision(
                        cExpectedAliveIndications,
                        cMinMargin,
                        cMaxMargin,
                        cAliveReferenceCycle,
                        cFailedReferenceCyclesTolerance),
                    std::invalid_argument);
            }

            TEST(AliveSupervisionTest, ZeroMaxMarginConstruction)
            {
                const uint16_t cExpectedAliveIndications{2};
                const uint16_t cMinMargin{1};
                const uint16_t cMaxMargin{0};
                const std::chrono::milliseconds cAliveReferenceCycle{1};
                const uint8_t cFailedReferenceCyclesTolerance{1};

                EXPECT_THROW(
                    AliveSupervision _supervision(
                        cExpectedAliveIndications,
                        cMinMargin,
                        cMaxMargin,
                        cAliveReferenceCycle,
                        cFailedReferenceCyclesTolerance),
                    std::invalid_argument);
            }

            TEST(AliveSupervisionTest, ZeroAliveReferenceCycleConstruction)
            {
                const uint16_t cExpectedAliveIndications{2};
                const uint16_t cMinMargin{1};
                const uint16_t cMaxMargin{1};
                const std::chrono::milliseconds cAliveReferenceCycle{0};
                const uint8_t cFailedReferenceCyclesTolerance{1};

                EXPECT_THROW(
                    AliveSupervision _supervision(
                        cExpectedAliveIndications,
                        cMinMargin,
                        cMaxMargin,
                        cAliveReferenceCycle,
                        cFailedReferenceCyclesTolerance),
                    std::invalid_argument);
            }

            TEST(AliveSupervisionTest, ZeroFailedReferenceCyclesToleranceConstruction)
            {
                const uint16_t cExpectedAliveIndications{2};
                const uint16_t cMinMargin{1};
                const uint16_t cMaxMargin{1};
                const std::chrono::milliseconds cAliveReferenceCycle{1};
                const uint8_t cFailedReferenceCyclesTolerance{0};

                EXPECT_THROW(
                    AliveSupervision _supervision(
                        cExpectedAliveIndications,
                        cMinMargin,
                        cMaxMargin,
                        cAliveReferenceCycle,
                        cFailedReferenceCyclesTolerance),
                    std::invalid_argument);
            }
        }
    }
}