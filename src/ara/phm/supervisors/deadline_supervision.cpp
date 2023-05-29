#include "./deadline_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            const TypeOfSupervision DeadlineSupervision::cSupervisionType{
                TypeOfSupervision::DeadlineSupervision};

            DeadlineSupervision::DeadlineSupervision(
                std::chrono::milliseconds minDeadline,
                std::chrono::milliseconds maxDeadline) : ElementarySupervision(cSupervisionType),
                                                         cMinDeadline{minDeadline < maxDeadline ? minDeadline : throw std::invalid_argument("Maximum deadline should be greater than the minimum deadline.")},
                                                         cMaxDeadline{maxDeadline.count() > 0 ? maxDeadline : throw std::invalid_argument("Maximum deadline should be greater than zero.")}
            {
            }

            void DeadlineSupervision::deadlineWorker()
            {
                std::unique_lock<std::mutex> _cvLock{mCvMutex};
                const std::cv_status cStatus{mDeadlineCv.wait_for(_cvLock, cMaxDeadline)};

                if (cStatus == std::cv_status::timeout || !mTargetIsReached)
                {
                    // If the maximum deadline is reached,
                    // or the worker is interrupted by the source checkpoint occurrence,
                    // then the supervision is failed

                    Report(SupervisionStatus::kExpired);
                }
                else
                {
                    // Otherwise, the worker is interrupted by the target checkpoint occurrence,
                    // and in this case it should be check whether the occurrence is before the minimum deadline or not
                    // and if it is, then the supervision is failed

                    const auto cTimeNow{std::chrono::steady_clock::now()};
                    const auto cTimeDelta{
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            cTimeNow - mTimeReference)};

                    if (cTimeDelta < cMinDeadline)
                    {
                        Report(SupervisionStatus::kExpired);
                    }
                    else
                    {
                        Report(SupervisionStatus::kOk);
                    }
                }
            }

            void DeadlineSupervision::ReportSourceCheckpoint()
            {
                mTimeReference = std::chrono::steady_clock::now();
                mTargetIsReached = false;

                if (mDeadlineThread.joinable())
                {
                    // If the worker is running, notify the source checkpoint is occurred.
                    mDeadlineCv.notify_one();
                }
                else
                {
                    // Otherwise, start the worker
                    auto _worker{
                        std::bind(&DeadlineSupervision::deadlineWorker, this)};
                    mDeadlineThread = std::thread(std::move(_worker));
                }
            }

            void DeadlineSupervision::ReportTargetCheckpoint()
            {
                mTargetIsReached = true;
                mDeadlineCv.notify_one();
            }

            DeadlineSupervision::~DeadlineSupervision()
            {
                if (mDeadlineThread.joinable())
                {
                    // Wait for the worker thread to be gracefully finished
                    mDeadlineThread.join();
                }
            }
        }
    }
}