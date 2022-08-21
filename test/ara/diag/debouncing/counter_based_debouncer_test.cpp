#include <gtest/gtest.h>
#include "../../../../src/ara/diag/debouncing/counter_based_debouncer.h"

namespace ara
{
    namespace diag
    {
        namespace debouncing
        {
            class CounterBasedDebouncerTest : public testing::Test
            {
            protected:
                EventStatus Status;

            public:
                CounterBasedDebouncerTest() : Status{EventStatus::kPending}
                {
                }

                void OnStatusChanged(bool passed)
                {
                    Status = passed ? EventStatus::kPassed : EventStatus::kFailed;
                }
            };

            TEST_F(CounterBasedDebouncerTest, PrepassWithoutJumpScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};

                CounterBased _defaultValues;
                _defaultValues.passedStepsize = 25;
                _defaultValues.passedThreshold = -75;
                _defaultValues.useJumpToPassed = false;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed(); // FDC: -25
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrepassed(); // FDC: -50
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrepassed(); // FDC: -75
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(CounterBasedDebouncerTest, PrepassWithJumpScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};

                CounterBased _defaultValues;
                _defaultValues.passedStepsize = 25;
                _defaultValues.passedThreshold = -75;
                _defaultValues.useJumpToPassed = true;
                _defaultValues.passedJumpValue = -50;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed(); // FDC: -50
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrepassed(); // FDC: -75
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(CounterBasedDebouncerTest, PassScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};

                CounterBased _defaultValues;
                _defaultValues.passedThreshold = -75;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPassed(); // FDC: -75
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(CounterBasedDebouncerTest, PrefailWithoutJumpScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};

                CounterBased _defaultValues;
                _defaultValues.failedStepsize = 25;
                _defaultValues.failedThreshold = 75;
                _defaultValues.useJumpToFailed = false;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrefailed(); // FDC: 25
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrefailed(); // FDC: 50
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrefailed(); // FDC: 75
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(CounterBasedDebouncerTest, PrefailWithJumpScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};

                CounterBased _defaultValues;
                _defaultValues.failedStepsize = 25;
                _defaultValues.failedThreshold = 75;
                _defaultValues.useJumpToFailed = true;
                _defaultValues.failedJumpValue = 50;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrefailed(); // FDC: 50
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrefailed(); // FDC: 75
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(CounterBasedDebouncerTest, FailScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};

                CounterBased _defaultValues;
                _defaultValues.failedThreshold = 75;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportFailed(); // FDC: 75
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(CounterBasedDebouncerTest, ResetMethod)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};

                CounterBased _defaultValues;
                _defaultValues.passedStepsize = 25;
                _defaultValues.passedThreshold = -50;
                _defaultValues.useJumpToPassed = false;

                auto _callback{
                    std::bind(
                        &CounterBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                CounterBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed(); // FDC: -25
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.Reset(); // FDC: 0

                _debouncer.ReportPrepassed(); // FDC: -25
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrepassed(); // FDC: -50
                EXPECT_EQ(cExpectedResult, Status);
            }
        }
    }
}