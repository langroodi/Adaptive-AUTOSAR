#include <gtest/gtest.h>
#include "../../../src/ara/diag/monitor.h"

namespace ara
{
    namespace diag
    {
        TEST(MonitorTest, CounterBasedOfferScenario)
        {
            core::InstanceSpecifier _specifier("Instance0");
            InitMonitorReason _currentReason{InitMonitorReason::kClear};
            auto _initMonitor = [&](InitMonitorReason newReason)
            {
                _currentReason = newReason;
            };
            CounterBased _defaultValues;

            Monitor _monitor(_specifier, _initMonitor, _defaultValues);

            core::Result<void> _result{_monitor.Offer()};
            EXPECT_TRUE(_result.HasValue());
            EXPECT_EQ(InitMonitorReason::kReenabled, _currentReason);

            core::Result<void> _newResult{_monitor.Offer()};
            EXPECT_FALSE(_newResult.HasValue());

            _monitor.StopOffer();
            EXPECT_EQ(InitMonitorReason::kDisabled, _currentReason);
        }

        TEST(MonitorTest, TimerBasedOfferScenario)
        {
            core::InstanceSpecifier _specifier("Instance0");
            InitMonitorReason _currentReason{InitMonitorReason::kClear};
            auto _initMonitor = [&](InitMonitorReason newReason)
            {
                _currentReason = newReason;
            };
            TimeBased _defaultValues;

            Monitor _monitor(_specifier, _initMonitor, _defaultValues);

            core::Result<void> _result{_monitor.Offer()};
            EXPECT_TRUE(_result.HasValue());
            EXPECT_EQ(InitMonitorReason::kReenabled, _currentReason);

            core::Result<void> _newResult{_monitor.Offer()};
            EXPECT_FALSE(_newResult.HasValue());

            _monitor.StopOffer();
            EXPECT_EQ(InitMonitorReason::kDisabled, _currentReason);
        }
    }
}