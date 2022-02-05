#ifndef TTL_TIMER_H
#define TTL_TIMER_H

#include <mutex>
#include <condition_variable>
#include <atomic>

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
                bool mRequested;
                bool mDisposing;
                std::atomic_bool mSignalFlag;
                uint32_t mTtl;

            public:
                TtlTimer() noexcept;
                TtlTimer(const TtlTimer &) = delete;
                TtlTimer &operator=(const TtlTimer &) = delete;
                ~TtlTimer() noexcept;

                /// @brief Indicate whether the service client is requested or not
                /// @returns True if the service client is requested, otherwise false
                /// @see SetRequested
                bool GetRequested() const noexcept;

                /// @brief Set the service requested status
                /// @param requested Service client requested status
                /// @see GetRequested
                void SetRequested(bool requested) noexcept;

                /// @brief Indicate whether the service server is offered or not
                /// @returns True if the service server is offered, otherwise false
                /// @see SetOffered
                bool GetOffered() const noexcept;

                /// @brief Set the service offered status
                /// @param ttl Received service offer entry TTL
                /// @see GetOffered
                /// @note Zero TTL indicates stop offering.
                void SetOffered(uint32_t ttl) noexcept;

                /// @brief Wait for a signal from SetRequested or SetOffered
                /// @see SetRequested(bool)
                /// @see SetOffered(uint32_t)
                void WaitForSignal();

                /// @brief Wait for the timer to expire or reset
                /// @returns True if the TTL is expired, otherwise false in case of timer reset
                bool WaitForExpiration();

                /// @brief Dispose the timer which causes all the waitings return immediately
                /// @remarks The side effect of this function call is irreversible.
                void Dispose() noexcept;
            };
        }
    }
}

#endif