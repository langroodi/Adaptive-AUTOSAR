#ifndef TIMER_BASED_DEBOUNCER_H
#define TIMER_BASED_DEBOUNCER_H

#include <stdint.h>
#include "./debouncer.h"

namespace ara
{
    namespace diag
    {
        /// @brief Time-based debouncing parameters
        struct TimeBased
        {
            /// @brief Time threshold in milliseconds to report passing
            uint32_t passedMs;
            /// @brief Time threshold in milliseconds to report failure
            uint32_t failedMs;
        };

        namespace debouncing
        {
            class TimerBasedDebouncer : public Debouncer
            {
            private:
                const TimeBased &mDefaultValues;

            public:
                /// @brief Constructor
                /// @param callback Callback to be triggered at the monitored event status change
                /// @param defaultValues Time-based debouncing default parameters
                TimerBasedDebouncer(
                    std::function<void(bool)> callback,
                    const TimeBased &defaultValues);

                virtual void ReportPrepassed() override;

                virtual void ReportPrefailed() override;

                virtual void Freeze() override;

                virtual void Reset() override;
            };
        }
    }
}

#endif