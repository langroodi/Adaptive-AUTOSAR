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
        /// @brief Contour-based debouncing parameters
        struct CounterBased
        {
            /// @brief Threshold to report failure
            int16_t failedThreshold;
            /// @brief Threshold to report passing
            int16_t passedThreshold;
            /// @brief Debounce counter increment step
            uint16_t failedStepsize;
            /// @brief Debounce counter decrement step
            uint16_t passedStepsize;
            /// @brief Debounce counter jump on a pre-failed report
            int16_t failedJumpValue;
            /// @brief Debounce counter jump on a pre-passed report
            int16_t passedJumpValue;
            /// @brief Indicates whether debounce counter should jump or not on a pre-failed report
            bool useJumpToFailed;
            /// @brief Indicates whether debounce counter should jump or not on a pre-passed report
            bool useJumpToPassed;
        };

        /// @brief Time-based debouncing parameters
        struct TimeBased
        {
            /// @brief Time threshold in milliseconds to report passing
            uint32_t passedMs;
            /// @brief Time threshold in milliseconds to report failure
            uint32_t failedMs;
        };

        /// @brief Monitoring re-initialization reason
        enum class InitMonitorReason : uint32_t
        {
            kClear = 0x00,     ///< Event was cleared
            kRestart = 0x01,   ///< Operation cycle of the event was restarted
            kReenabled = 0x02, ///< Enable conditions (if set) are fullfilled
            kDisabled = 0x03   ///< Enable conditions (if set) are no longer fullfilled
        };

        /// @brief Diagnostic monitor action type reported by an adpative application
        enum class MonitorAction : uint32_t
        {
            kPassed = 0x00,              ///< Monitoring passed
            kFailed = 0x01,              ///< Monitoring failed
            kPrepassed = 0x02,           ///< Monitoring pre-passed
            kPrefailed = 0x03,           ///< Monitoring pre-failed
            kFdcThresholdReached = 0x04, ///< Fault Detection Counter (FDC) threshold has been reached
            kResetTestFailed = 0x05,     ///< Reset TestFailed bit in UDS DTC status
            kFreezeDebouncing = 0x06,    ///< Freeze the internal debouncing
            kResetDebouncing = 0x07      ///< Reset the internal debouncing
        };

        /// @brief A class to monitor the correct functionality of a system part
        class Mointor final
        {
        public:
            /// @brief Monitor constructor with an internal debouncing
            /// @param specifier Instance specifer that owns the monitor
            /// @param initMonitor Monitor re-initialization callback
            /// @param getFaultDetectionCounter Delegate to get the event Fault Detection Counter (FDC)
            Mointor(
                const ara::core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                std::function<int8_t()> getFaultDetectionCounter);

            /// @brief Monitor constructor with a counter-based debouncing
            /// @param specifier Instance specifer that owns the monitor
            /// @param initMonitor Monitor re-initialization callback
            /// @param defaultValues Counter-based debouncing default parameters
            Monitor(
                const ara::core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                CounterBased defaultValues);

            /// @brief Monitor constructor with a time-based debouncing
            /// @param specifier Instance specifer that owns the monitor
            /// @param initMonitor Monitor re-initialization callback
            /// @param defaultValues Time-based debouncing default parameters
            Monitor(
                const ara::core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                TimeBased defaultValues);

            /// @brief Report a monitor action
            /// @param action Latest diagnostic monitor action
            void ReportMonitorAction(MonitorAction action);

            /// @brief Start offering monitoring requests handling
            /// @returns Error result if the handler has been already offered
            ara::core::Result<void> Offer();

            /// @brief Stop offering monitoring requests handling
            void StopOffer();
        };
    }
}

#endif