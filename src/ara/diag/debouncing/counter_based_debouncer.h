#ifndef COUNTER_BASED_DEBOUNCER_H
#define COUNTER_BASED_DEBOUNCER_H

#include <stdint.h>
#include "./debouncer.h"

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

        namespace debouncing
        {
            class CounterBasedDebouncer : public Debouncer
            {
            private:
                const CounterBased mDefaultValues;
                int16_t mFdc;
                
            public:
                /// @brief Constructor
                /// @param callback Callback to be triggered at the monitored event status change
                /// @param defaultValues Counter-based debouncing default parameters
                CounterBasedDebouncer(
                    std::function<void(bool)> callback,
                    CounterBased defaultValues);

                virtual void ReportPrepassed() override;

                virtual void ReportPassed() override;

                virtual void ReportPrefailed() override;

                virtual void ReportFailed() override;

                virtual void Freeze() override;

                virtual void Reset() override;
            };
        }
    }
}

#endif