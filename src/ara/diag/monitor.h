#ifndef MONITOR_H
#define MONITOR_H

#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "./debouncing/counter_based_debouncer.h"
#include "./debouncing/timer_based_debouncer.h"
#include "./event.h"

namespace ara
{
    namespace diag
    {
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
        /// @note In constrast with the ARA standard, internal debouncing is not supported.
        class Monitor final
        {
        private:
            const core::InstanceSpecifier &mSpecifier;
            const std::function<void(InitMonitorReason)> mInitMonitor;
            bool mOffered;
            debouncing::Debouncer *mDebouncer;
            Event *mEvent;

            Monitor(
                const core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor);

            void onEventStatusChanged(bool passed);

        public:
            /// @brief Monitor constructor with a counter-based debouncing
            /// @param specifier Instance specifer that owns the monitor
            /// @param initMonitor Monitor re-initialization callback
            /// @param defaultValues Counter-based debouncing default parameters
            Monitor(
                const core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                CounterBased defaultValues);

            /// @brief Monitor constructor with a time-based debouncing
            /// @param specifier Instance specifer that owns the monitor
            /// @param initMonitor Monitor re-initialization callback
            /// @param defaultValues Time-based debouncing default parameters
            Monitor(
                const core::InstanceSpecifier &specifier,
                std::function<void(InitMonitorReason)> initMonitor,
                TimeBased defaultValues);

            ~Monitor() noexcept;

            /// @brief Report a monitor action
            /// @param action Latest diagnostic monitor action
            /// @throws std::invalid_argument Throws when the action is not supported
            void ReportMonitorAction(MonitorAction action);

            /// @brief Attach an event to the monitor object
            /// @param event Event pointer to be attached
            /// @note This function is not part of the ARA standard.
            void AttachEvent(Event *event);

            /// @brief Start offering monitoring requests handling
            /// @returns Error result if the handler has been already offered
            core::Result<void> Offer();

            /// @brief Stop offering monitoring requests handling
            void StopOffer();
        };
    }
}

#endif