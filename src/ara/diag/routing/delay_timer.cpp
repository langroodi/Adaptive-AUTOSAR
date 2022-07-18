#include "./delay_timer.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            DelayTimer::DelayTimer() noexcept : mLock(mMutex, std::defer_lock)
            {
            }

            void DelayTimer::delay(std::chrono::seconds duration)
            {
                mLock.lock();
                mConditionVariable.wait_for(mLock, duration);
                mLock.unlock();
            }

            void DelayTimer::Start(std::chrono::seconds delayDuration)
            {
                if (!IsActive())
                {
                    mThread = std::thread(&DelayTimer::delay, this, delayDuration);
                    // Spinning till the timer become activated.
                    while (!IsActive())
                    {
                        std::this_thread::sleep_for(cSpinWait);
                    }
                }
            }

            bool DelayTimer::IsActive() const noexcept
            {
                return mLock.owns_lock();
            }

            void DelayTimer::Dispose()
            {
                if (mThread.joinable())
                {
                    mConditionVariable.notify_one();
                    mThread.join();
                }
            }

            DelayTimer::~DelayTimer()
            {
                Dispose();
            }
        }
    }
}