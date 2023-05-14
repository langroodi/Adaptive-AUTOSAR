#ifndef DEADLINE_SUPERVISION_H
#define DEADLINE_SUPERVISION_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            /// @brief Supervision method to check an entity aliveness via the source and target checkpoints
            class DeadlineSupervision
            {
            private:
                const std::chrono::milliseconds cMinDeadline;
                const std::chrono::milliseconds cMaxDeadline;

                std::function<void()> mCallback;
                std::thread mDeadlineThread;
                bool mTargetIsReached;
                std::mutex mCvMutex;
                std::condition_variable mDeadlineCv;
                std::chrono::time_point<std::chrono::steady_clock> mTimeReference;

                void deadlineWorker();

            public:
                /// @brief Constructor
                /// @param minDeadline Source to target checkpoint transition checkpoint mimimum deadline
                /// @param maxDeadline Source to target checkpoint transition checkpoint maximum deadline
                /// @param callback Callback to be invoked when the supervision failed
                /// @throws std::invalid_argument Thrown when the deadline values are invalid
                DeadlineSupervision(
                    std::chrono::milliseconds minDeadline,
                    std::chrono::milliseconds maxDeadline,
                    std::function<void()> &&callback);

                DeadlineSupervision() = delete;
                ~DeadlineSupervision();

                /// @brief Report the occurrence of the source checkpoint
                void ReportSourceCheckpoint();

                /// @brief Report the occurrence of the target checkpoint
                void ReportTargetCheckpoint();
            };
        }
    }
}

#endif