#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>
#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        struct CounterBased
        {
            std::int16_t failedThreshold;
            std::int16_t passedThreshold;
            std::uint16_t failedStepsize;
            std::uint16_t passedStepsize;
            std::int16_t failedJumpValue;
            std::int16_t passedJumpValue;
            bool useJumpToFailed;
            bool useJumpToPassed;
        };

        struct TimeBased
        {
            std::uint32_t passedMs;
            std::uint32_t failedMs;
        };

        enum class InitMonitorReason
        {
            kClear = 0x00,
            kRestart = 0x01,
            kReenabled = 0x02
        };

        enum class MonitorAction
        {
            kPassed = 0x00,
            kFailed = 0x01,
            kPrepassed = 0x02,
            kPrefailed = 0x03,
            kFdcThresholdReached = 0x04,
            kResetTestFailed = 0x05,
            kFreezeDebouncing = 0x06,
            kResetDebouncing = 0x07
        };

        class Mointor final
        {
        public:
            Mointor(
                const ara::core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                std::function<std::int8_t()> getFaultDetectionCounter);
            Monitor(
                const ara::core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                CounterBased debouncing);
            Monitor(
                const ara::core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                TimeBased debouncing);
            ara::core::Result<void> ReportMonitorAction(MonitorAction action);
        };
    }
}

#endif