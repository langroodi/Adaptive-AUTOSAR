#include <gtest/gtest.h>
#include "../../../../src/ara/phm/supervisors/deadline_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            static const int cNyquistMargin = 2;

            TEST(DeadlineSupervisionTest, InvalidMinDeadlineConstruction)
            {
                const std::chrono::milliseconds cMinDeadline{2};
                const std::chrono::milliseconds cMaxDeadline{2};

                EXPECT_THROW(
                    DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline),
                    std::invalid_argument);
            }

            TEST(DeadlineSupervisionTest, ZeroMaxDeadlineConstruction)
            {
                const std::chrono::milliseconds cMinDeadline{-1};
                const std::chrono::milliseconds cMaxDeadline{0};

                EXPECT_THROW(
                    DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline),
                    std::invalid_argument);
            }

            TEST(DeadlineSupervisionTest, DeadlinePassScenario)
            {
                const std::chrono::milliseconds cMinDeadline{2};
                const std::chrono::milliseconds cTargetReportTime{cMinDeadline * cNyquistMargin};
                const std::chrono::milliseconds cMaxDeadline{cTargetReportTime * cNyquistMargin};

                bool _failed{false};

                DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline);

                _supervision.SetCallback([&](SupervisionStatus status)
                                         { _failed = status == SupervisionStatus::kExpired; });
                _supervision.ReportSourceCheckpoint();
                std::this_thread::sleep_for(cTargetReportTime);
                _supervision.ReportTargetCheckpoint();
                std::this_thread::sleep_for(cMaxDeadline * cNyquistMargin);

                EXPECT_FALSE(_failed);
            }

            TEST(DeadlineSupervisionTest, IgnoreTargetCheckpointScenario)
            {
                const std::chrono::milliseconds cMinDeadline{2};
                const std::chrono::milliseconds cMaxDeadline{3};

                bool _failed{false};

                DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline);

                _supervision.SetCallback([&](SupervisionStatus status)
                                         { _failed = status == SupervisionStatus::kExpired; });
                _supervision.ReportTargetCheckpoint();

                EXPECT_FALSE(_failed);
            }

            TEST(DeadlineSupervisionTest, DoubleSourceCheckpointReportScenario)
            {
                const std::chrono::milliseconds cMinDeadline{2};
                const std::chrono::milliseconds cTargetReportTime{cMinDeadline * cNyquistMargin};
                const std::chrono::milliseconds cMaxDeadline{cTargetReportTime * cNyquistMargin};

                bool _failed{false};

                DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline);

                _supervision.SetCallback([&](SupervisionStatus status)
                                         { _failed = status == SupervisionStatus::kExpired; });
                _supervision.ReportSourceCheckpoint();
                std::this_thread::sleep_for(cTargetReportTime);
                _supervision.ReportSourceCheckpoint();
                std::this_thread::sleep_for(cMaxDeadline * cNyquistMargin);

                EXPECT_TRUE(_failed);
            }

            TEST(DeadlineSupervisionTest, MinDeadlineFailScenario)
            {
                const std::chrono::milliseconds cMinDeadline{2};
                const std::chrono::milliseconds cMaxDeadline{3};

                bool _failed{false};

                DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline);

                _supervision.SetCallback([&](SupervisionStatus status)
                                         { _failed = status == SupervisionStatus::kExpired; });
                _supervision.ReportSourceCheckpoint();
                _supervision.ReportTargetCheckpoint();

                std::this_thread::sleep_for(cMaxDeadline * cNyquistMargin);

                EXPECT_TRUE(_failed);
            }

            TEST(DeadlineSupervisionTest, MaxDeadlineFailScenario)
            {
                const std::chrono::milliseconds cMinDeadline{2};
                const std::chrono::milliseconds cMaxDeadline{3};

                bool _failed{false};

                DeadlineSupervision _supervision(cMinDeadline, cMaxDeadline);

                _supervision.SetCallback([&](SupervisionStatus status)
                                         { _failed = status == SupervisionStatus::kExpired; });
                _supervision.ReportSourceCheckpoint();
                std::this_thread::sleep_for(cMaxDeadline * cNyquistMargin);
                _supervision.ReportTargetCheckpoint();

                EXPECT_TRUE(_failed);
            }
        }
    }
}