#ifndef ANNOUNCEMENT_TIMER_H
#define ANNOUNCEMENT_TIMER_H

#include <thread>
#include <functional>
#include <condition_variable>

namespace DoipLib
{
    /// @brief A timer to announce the vehicle for a specific timer with a specific delay in between
    class AnnouncementTimer
    {
    private:
        const std::chrono::microseconds cSpinWait{1};

        std::mutex mMutex;
        std::unique_lock<std::mutex> mLock;
        std::condition_variable mConditionVariable;
        std::chrono::seconds mDelay;
        std::chrono::seconds mInterval;
        uint8_t mCount;
        std::thread mThread;
        std::function<void()> mCallback;

        void announce();
        void dispose();

    public:
        /// @brief Constructor
        /// @param delay Delay in seconds before starting the announcement
        /// @param interval Interval in seconds between each announcement
        /// @param count Number of annoucement broadcasting iterations
        AnnouncementTimer(
            std::chrono::seconds delay,
            std::chrono::seconds interval,
            uint8_t count);
        AnnouncementTimer(const AnnouncementTimer &) = delete;
        AnnouncementTimer &operator=(const AnnouncementTimer &) = delete;
        ~AnnouncementTimer();

        /// @brief (Re)start the timer to tick
        /// @param callback Delegate to be invoked on each timer tick
        /// @note The caller will be blocked until the timer has been started.
        void Start(std::function<void()> &&callback);
    };
}

#endif