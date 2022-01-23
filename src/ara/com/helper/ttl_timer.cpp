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
                                            mTtl{0}
            {
            }

            void TtlTimer::WaitForSignal()
            {
                mLock.lock();
                mConditionVariable.wait(mLock);
                mLock.unlock();
            }

            bool TtlTimer::Wait()
            {
                mLock.lock();
                std::cv_status _status =
                    mConditionVariable.wait_for(
                        mLock, std::chrono::seconds(mTtl));
                mLock.unlock();

                bool _result = _status == std::cv_status::no_timeout;
                return _result;
            }

            void TtlTimer::Set(uint32_t ttl) noexcept
            {
                mTtl = ttl;
                mConditionVariable.notify_one();
            }

            void TtlTimer::Cancel() noexcept
            {
                mConditionVariable.notify_one();
            }

            TtlTimer::~TtlTimer() noexcept
            {
                mConditionVariable.notify_one();
            }
        }
    }
}