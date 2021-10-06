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
                std::mutex mMutex;
                std::unique_lock<std::mutex> mLock;
                std::condition_variable mConditionVariable;
                std::future<void> mFuture;
                bool mRunning;
                uint32_t mTtl;
                std::function<void()> mOnExpired;

                void countdown();

            public:
                TtlTimer() noexcept;
                TtlTimer(const TtlTimer &) = delete;
                TtlTimer &operator=(const TtlTimer &) = delete;
                ~TtlTimer();

                /// @brief Set a callback to be invoked on the timer's expiration
                /// @param callback Callback to be invoked on the expiration from the timer's thread
                void SetExpirationCallback(std::function<void()> callback);

                /// @brief Reset the timer's expiration callback
                void ResetExpirationCallback() noexcept;

                /// @brief Set the TTL timer
                /// @param ttl Time to live in seconds
                /// @throws std::logic_error Throws when the timer has been set already. Call Reset(uint32_t) instead.
                void Set(uint32_t ttl);

                /// @brief Reset the TTL timer
                /// @param ttl New time to live in seconds
                void Reset(uint32_t ttl);

                /// @brief Cancel the timer
                /// @note The function blocks the caller till the timer stops.
                void Cancel();
            };
        }
    }
}

#endif