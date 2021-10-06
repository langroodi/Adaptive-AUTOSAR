#ifndef TTL_TIMER_H
#define TTL_TIMER_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

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
                const std::function<void()> mOnExpired;
                std::mutex mMutex;
                std::unique_lock<std::mutex> mLock;
                std::condition_variable mConditionVariable;
                std::future<void> mFuture;
                bool mRunning;
                int mTtl;

                void countdown();

            public:
                /// @brief Constructor
                /// @param onExpired Callback to be invoked when the TTL expires
                TtlTimer(std::function<void()> onExpired) noexcept;

                TtlTimer() = delete;
                TtlTimer(const TtlTimer &) = delete;
                TtlTimer &operator=(const TtlTimer &) = delete;
                ~TtlTimer();

                /// @brief Set the TTL timer
                /// @param ttl Time to live in seconds
                /// @throws std::logic_error Throws when the timer has been set already. Call Reset(int) instead.
                void Set(int ttl);

                /// @brief Reset the TTL timer
                /// @param ttl New time to live in seconds
                void Reset(int ttl);

                /// @brief Cancel the timer
                /// @note The function blocks the caller till the timer stops.
                void Cancel();
            };
        }
    }
}

#endif