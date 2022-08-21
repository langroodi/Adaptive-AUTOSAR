#ifndef TIMER_BASED_DEBOUNCER_H
#define TIMER_BASED_DEBOUNCER_H

#include <stdint.h>
#include <thread>
#include <condition_variable>
#include <atomic>
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
                const TimeBased mDefaultValues;

                std::mutex mMutex;
                std::unique_lock<std::mutex> mLock;
                std::condition_variable mConditionVariable;
                std::thread mThread;
                std::atomic_uint32_t mElapsedMs;
                bool mIsPassing;

                void tick(std::chrono::milliseconds duration, bool passing);
                void start(uint32_t threshold);

            public:
                /// @brief Constructor
                /// @param callback Callback to be triggered at the monitored event status change
                /// @param defaultValues Time-based debouncing default parameters
                TimerBasedDebouncer(
                    std::function<void(bool)> callback,
                    TimeBased defaultValues);
                
                virtual ~TimerBasedDebouncer() override;

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