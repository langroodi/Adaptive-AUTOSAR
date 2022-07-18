#ifndef DELAY_TIMER_H
#define DELAY_TIMER_H

#include <thread>
#include <condition_variable>

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            /// @brief A thread-safe countdown timer running on a different thread
            class DelayTimer
            {
            private:
                const std::chrono::microseconds cSpinWait{1};

                std::mutex mMutex;
                std::unique_lock<std::mutex> mLock;
                std::condition_variable mConditionVariable;
                std::thread mThread;

                void delay(std::chrono::seconds duration);

            public:
                DelayTimer() noexcept;
                DelayTimer(const DelayTimer &) = delete;
                DelayTimer &operator=(const DelayTimer &) = delete;
                ~DelayTimer();

                /// @brief Start the timer if it has not been started yet
                /// @param delayDuration Timer delay duration in [sec]
                /// @note The caller will be blocked until the timer has been started.
                void Start(std::chrono::seconds delayDuration);

                /// @brief Indicate whether the timer is active or not
                /// @returns True if the timer is active, otherwise false
                bool IsActive() const noexcept;

                /// @brief Release resources aquired by the timer
                void Dispose();
            };
        }
    }
}

#endif