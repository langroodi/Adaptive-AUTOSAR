#include <gtest/gtest.h>
#include "../../../../src/ara/diag/debouncing/timer_based_debouncer.h"

namespace ara
{
    namespace diag
    {
        namespace debouncing
        {
            class TimerBasedDebouncerTest : public testing::Test
            {
            protected:
                EventStatus Status;

            public:
                TimerBasedDebouncerTest() : Status{EventStatus::kPending}
                {
                }

                void OnStatusChanged(bool passed)
                {
                    Status = passed ? EventStatus::kPassed : EventStatus::kFailed;
                }
            };

            TEST_F(TimerBasedDebouncerTest, PrepassScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.passedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed();
                EXPECT_NE(cExpectedResult, Status);

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);

                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, PassScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.passedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPassed();
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, PrefailScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.failedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrefailed();
                EXPECT_NE(cExpectedResult, Status);

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);

                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, FailScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.failedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportFailed();
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, PrepassPrepassScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.passedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed();
                EXPECT_NE(cExpectedResult, Status);

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);

                _debouncer.ReportPrepassed();
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, PrefailPrefailScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.failedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrefailed();
                EXPECT_NE(cExpectedResult, Status);

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);

                _debouncer.ReportPrefailed();
                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, PrepassPrefailScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kFailed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.passedMs = cThreshold;
                _defaultValues.failedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed();
                std::chrono::milliseconds _halfThreshold(cThreshold / 2);
                std::this_thread::sleep_for(_halfThreshold);
                _debouncer.ReportPrefailed();

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);

                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, PrefailPrepassScenario)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.passedMs = cThreshold;
                _defaultValues.failedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrefailed();
                std::chrono::milliseconds _halfThreshold(cThreshold / 2);
                std::this_thread::sleep_for(_halfThreshold);
                _debouncer.ReportPrepassed();

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);

                EXPECT_EQ(cExpectedResult, Status);
            }

            TEST_F(TimerBasedDebouncerTest, FreezeMethod)
            {
                const EventStatus cExpectedResult{EventStatus::kPassed};
                const uint32_t cThreshold{50};

                TimeBased _defaultValues;
                _defaultValues.passedMs = cThreshold;

                auto _callback{
                    std::bind(
                        &TimerBasedDebouncerTest::OnStatusChanged,
                        this, std::placeholders::_1)};

                TimerBasedDebouncer _debouncer(_callback, _defaultValues);

                _debouncer.ReportPrepassed();
                std::chrono::milliseconds _halfThreshold(cThreshold / 2);
                std::this_thread::sleep_for(_halfThreshold);
                _debouncer.Freeze();

                // Wait twice the threshold according Nyquist theorem to check the result
                std::chrono::milliseconds _ms(cThreshold * 2);
                std::this_thread::sleep_for(_ms);
                EXPECT_NE(cExpectedResult, Status);

                _debouncer.ReportPrepassed();
                std::this_thread::sleep_for(_ms);
                EXPECT_EQ(cExpectedResult, Status);
            }
        }
    }
}