#include <chrono>
#include <stdexcept>
#include "./ttl_timer.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            TtlTimer::TtlTimer() noexcept : mLock(mMutex, std::defer_lock),
                                            mRunning{false}
            {
            }

            void TtlTimer::countdown()
            {
                while (mRunning)
                {
                    mLock.lock();
                    std::cv_status _status =
                        mConditionVariable.wait_for(
                            mLock, std::chrono::seconds(mTtl));
                    mLock.unlock();

                    // Invoke the timer expiration callback if timeout occurred
                    if (_status == std::cv_status::timeout)
                    {
                        mRunning = false;
                        if (mOnExpired)
                        {
                            mOnExpired();
                        }
                    }
                }
            }

            void TtlTimer::SetExpirationCallback(std::function<void()> callback)
            {
                mOnExpired = callback;
            }

            void TtlTimer::ResetExpirationCallback() noexcept
            {
                mOnExpired = std::function<void()>();
            }

            void TtlTimer::Set(int ttl)
            {
                if (mRunning)
                {
                    throw std::logic_error("The timer has been set already.");
                }
                else
                {
                    mTtl = ttl;
                    mRunning = true;
                    mFuture =
                        std::async(std::launch::async, &TtlTimer::countdown, this);
                }
            }

            void TtlTimer::Reset(int ttl)
            {
                if (mRunning)
                {
                    mTtl = ttl;
                    mConditionVariable.notify_one();
                }
                else
                {
                    Set(ttl);
                }
            }

            void TtlTimer::Cancel()
            {
                mRunning = false;
                mConditionVariable.notify_one();

                // Wait for the timer's thread to cancel the timer gracefully
                if (mFuture.valid())
                {
                    mFuture.get();
                }
            }

            TtlTimer::~TtlTimer()
            {
                // Cancel the timer before destruction if it is still running
                if (mRunning)
                {
                    Cancel();
                }
            }
        }
    }
}