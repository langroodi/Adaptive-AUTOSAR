#ifndef TTL_TIMER_H
#define TTL_TIMER_H

#include <mutex>
#include <condition_variable>

namespace ara
{
    namespace com
    {
        namespace helper
        {
            /// @brief Time To Live countdown timer
            /// @note The timer is not copyable.
            class TtlTimer
            {
            private:
                std::mutex mMutex;
                std::unique_lock<std::mutex> mLock;
                std::condition_variable mConditionVariable;
                uint32_t mTtl;

            public:
                TtlTimer() noexcept;
                TtlTimer(const TtlTimer &) = delete;
                TtlTimer &operator=(const TtlTimer &) = delete;
                ~TtlTimer() noexcept;

                /// @brief Block till Cancel or Set method is called
                /// @see Cancel
                /// @see Set
                void WaitForSignal();

                /// @brief Wait for the timer to expire or reset
                /// @returns True if the timer is reset, other false in case of expiration
                bool Wait();

                /// @brief Set the TTL timer
                /// @param ttl Time to live in seconds
                void Set(uint32_t ttl) noexcept;

                /// @brief Cancel the timer immediately
                void Cancel() noexcept;
            };
        }
    }
}

#endif