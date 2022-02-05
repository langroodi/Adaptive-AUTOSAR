#include <chrono>
#include "./ttl_timer.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            TtlTimer::TtlTimer() noexcept : mLock(mMutex, std::defer_lock),
                                            mRequested{false},
                                            mDisposing{false},
                                            mTtl{0}
            {
            }

            bool TtlTimer::GetRequested() const noexcept
            {
                return mRequested;
            }

            void TtlTimer::SetRequested(bool requested) noexcept
            {
                mRequested = requested;
                mSignalFlag = true;
                mConditionVariable.notify_one();
            }

            bool TtlTimer::GetOffered() const noexcept
            {
                return mTtl > 0;
            }

            void TtlTimer::SetOffered(uint32_t ttl) noexcept
            {
                mTtl = ttl;
                mSignalFlag = true;
                mConditionVariable.notify_one();
            }

            void TtlTimer::WaitForSignal()
            {
                // If disposing, return immediately
                if (mDisposing)
                {
                    return;
                }
                else
                {
                    mLock.lock();
                    mConditionVariable.wait(mLock, [this]{ return mSignalFlag.load(); });
                    mSignalFlag = false;
                    mLock.unlock();
                }
            }

            bool TtlTimer::WaitForExpiration()
            {
                // If disposing, return timeout immediately
                if (mTtl == 0 || mDisposing)
                {
                    return true;
                }
                else
                {
                    mLock.lock();
                    std::cv_status _status =
                        mConditionVariable.wait_for(
                            mLock, std::chrono::seconds(mTtl));
                    mSignalFlag = false;
                    mLock.unlock();

                    // Due to the TTL delay above, re-check the TTL and disposing values
                    if (_status == std::cv_status::timeout || mTtl == 0 || mDisposing)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            void TtlTimer::Dispose() noexcept
            {
                mDisposing = true;
            }

            TtlTimer::~TtlTimer() noexcept
            {
                Dispose();
                mConditionVariable.notify_one();
            }
        }
    }
}