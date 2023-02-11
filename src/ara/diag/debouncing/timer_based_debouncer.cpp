#include <chrono>
#include "./timer_based_debouncer.h"

namespace ara
{
    namespace diag
    {
        namespace debouncing
        {
            TimerBasedDebouncer::TimerBasedDebouncer(
                std::function<void(bool)> callback,
                TimeBased defaultValues) : Debouncer(callback),
                                           mLock(mMutex, std::defer_lock),
                                           mDefaultValues{defaultValues},
                                           mElapsedMs{0}
            {
            }

            void TimerBasedDebouncer::tick(
                std::chrono::milliseconds duration, bool passing)
            {
                std::chrono::steady_clock::time_point _beginTime{
                    std::chrono::steady_clock::now()};

                mLock.lock();
                std::cv_status _status{mConditionVariable.wait_for(mLock, duration)};
                mLock.unlock();

                if (_status == std::cv_status::timeout)
                {
                    SetEventStatus(
                        passing ? EventStatus::kPassed : EventStatus::kFailed);

                    mElapsedMs =
                        passing ? mDefaultValues.passedMs : mDefaultValues.failedMs;
                }
                else
                {
                    std::chrono::steady_clock::time_point _endTime{
                        std::chrono::steady_clock::now()};

                    mElapsedMs =
                        static_cast<uint32_t>(
                            std::chrono::duration_cast<std::chrono::milliseconds>(
                                _endTime - _beginTime)
                                .count());
                }
            }

            void TimerBasedDebouncer::start(uint32_t threshold)
            {
                const std::chrono::microseconds cSpinWait{1};

                std::chrono::milliseconds _duration(threshold - mElapsedMs);
                if (!mLock.owns_lock() && _duration.count() > 0)
                {
                    if (mThread.joinable())
                    {
                        mThread.join();
                    }
                    
                    mThread =
                        std::thread(
                            &TimerBasedDebouncer::tick, this, _duration, mIsPassing);

                    // Spinning till the timer become activated.
                    while (!mLock.owns_lock())
                    {
                        std::this_thread::sleep_for(cSpinWait);
                    }
                }
            }

            void TimerBasedDebouncer::ReportPrepassed()
            {
                if (!mIsPassing)
                {
                    Freeze();
                    mElapsedMs = 0;
                    mIsPassing = true;
                }

                start(mDefaultValues.passedMs);
            }

            void TimerBasedDebouncer::ReportPassed()
            {
                Freeze();
                mElapsedMs = mDefaultValues.passedMs;
                mIsPassing = true;
                SetEventStatus(EventStatus::kPassed);
            }

            void TimerBasedDebouncer::ReportPrefailed()
            {
                if (mIsPassing)
                {
                    Freeze();
                    mElapsedMs = 0;
                    mIsPassing = false;
                }

                start(mDefaultValues.failedMs);
            }

            void TimerBasedDebouncer::ReportFailed()
            {
                Freeze();
                mElapsedMs = mDefaultValues.failedMs;
                mIsPassing = false;
                SetEventStatus(EventStatus::kFailed);
            }

            void TimerBasedDebouncer::Freeze()
            {
                if (mThread.joinable())
                {
                    mConditionVariable.notify_one();
                    mThread.join();
                }
            }

            void TimerBasedDebouncer::Reset()
            {
                Freeze();
                mElapsedMs = 0;
                SetEventStatus(EventStatus::kPending);
            }

            TimerBasedDebouncer::~TimerBasedDebouncer()
            {
                Reset();
            }
        }
    }
}