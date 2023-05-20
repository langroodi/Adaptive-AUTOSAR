#include <gtest/gtest.h>
#include "./dummy_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            TEST(ElementarySupervisionTest, Constructor)
            {
                const SupervisionStatus cExpectedResult{SupervisionStatus::kDeactivated};

                const DummySupervision cSupervision;
                const SupervisionStatus cActualResult{cSupervision.GetStatus()};

                EXPECT_EQ(cExpectedResult, cActualResult);
            }

            TEST(ElementarySupervisionTest, TransitionToOkScenario)
            {
                const SupervisionStatus cExpectedResult{SupervisionStatus::kOk};

                DummySupervision _supervision;

                _supervision.ReportStatus(cExpectedResult);
                SupervisionStatus _actualResult{_supervision.GetStatus()};
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to Failed and then to OK
                _supervision.ReportStatus(SupervisionStatus::kFailed);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to Expired and then to OK
                _supervision.ReportStatus(SupervisionStatus::kExpired);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_NE(cExpectedResult, _actualResult);
            }

            TEST(ElementarySupervisionTest, TransitionToFailedScenario)
            {
                const SupervisionStatus cExpectedResult{SupervisionStatus::kFailed};

                DummySupervision _supervision;

                _supervision.ReportStatus(cExpectedResult);
                SupervisionStatus _actualResult{_supervision.GetStatus()};
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to OK and then to Failed
                _supervision.ReportStatus(SupervisionStatus::kOk);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to Expired and then to Failed
                _supervision.ReportStatus(SupervisionStatus::kExpired);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_NE(cExpectedResult, _actualResult);
            }

            TEST(ElementarySupervisionTest, TransitionToExpiredScenario)
            {
                const SupervisionStatus cExpectedResult{SupervisionStatus::kExpired};

                DummySupervision _supervision;

                _supervision.ReportStatus(cExpectedResult);
                SupervisionStatus _actualResult{_supervision.GetStatus()};
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to OK and then to Expired
                _supervision.ReportStatus(SupervisionStatus::kOk);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to Failed and then to Expired
                _supervision.ReportStatus(SupervisionStatus::kFailed);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);
            }

            TEST(ElementarySupervisionTest, TransitionToDeactivatedScenario)
            {
                const SupervisionStatus cExpectedResult{SupervisionStatus::kDeactivated};

                DummySupervision _supervision;

                _supervision.ReportStatus(cExpectedResult);
                SupervisionStatus _actualResult{_supervision.GetStatus()};
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to OK and then to Deactivated
                _supervision.ReportStatus(SupervisionStatus::kOk);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to Failed and then to Deactivated
                _supervision.ReportStatus(SupervisionStatus::kFailed);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);

                // Transit to Expired and then to Deactivated
                _supervision.ReportStatus(SupervisionStatus::kExpired);
                _supervision.ReportStatus(cExpectedResult);
                _actualResult = _supervision.GetStatus();
                EXPECT_EQ(cExpectedResult, _actualResult);
            }
        }
    }
}