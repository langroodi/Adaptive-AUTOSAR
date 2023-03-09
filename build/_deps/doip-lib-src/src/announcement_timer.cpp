#include <utility>
#include <stdexcept>
#include "doiplib/announcement_timer.h"

namespace DoipLib
{
    void AnnouncementTimer::announce()
    {
        mLock.lock();

        std::cv_status _status{mConditionVariable.wait_for(mLock, mDelay)};

        // Start the annoucement after the delay if it is NOT interrupted
        if (_status == std::cv_status::timeout)
        {
            for (uint8_t i = 0; i < mCount; ++i)
            {
                _status = mConditionVariable.wait_for(mLock, mInterval);

                // Invoke the callback if the timer is NOT interrupted, otherwise
                // break the annoucement iteration loop and release the lock
                if (_status == std::cv_status::timeout)
                {
                    mCallback();
                }
                else
                {
                    break;
                }
            }
        }

        mLock.unlock();
    }

    AnnouncementTimer::AnnouncementTimer(
        std::chrono::seconds delay,
        std::chrono::seconds interval,
        uint8_t count) : mCount{count > 0 ? count : throw std::out_of_range("Announcement count should be greater than zero.")},
                          mLock(mMutex, std::defer_lock),
                          mDelay{delay},
                          mInterval{interval}
    {
    }

    void AnnouncementTimer::Start(std::function<void()> &&callback)
    {
        if (mLock.owns_lock())
        {
            dispose();
        }

        mCallback = std::move(callback);
        mThread = std::thread(&AnnouncementTimer::announce, this);

        // Spinning till the timer become activated.
        while (!mLock.owns_lock())
        {
            std::this_thread::sleep_for(cSpinWait);
        }
    }

    void AnnouncementTimer::dispose()
    {
        if (mThread.joinable())
        {
            mConditionVariable.notify_one();
            mThread.join();
        }
    }

    AnnouncementTimer::~AnnouncementTimer()
    {
        dispose();
    }
}