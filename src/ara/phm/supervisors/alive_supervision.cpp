#include <stdexcept>
#include <thread>
#include "./alive_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            const TypeOfSupervision AliveSupervision::cSupervisionType{
                TypeOfSupervision::AliveSupervision};

            AliveSupervision::AliveSupervision(
                uint16_t expectedAliveIndications,
                uint16_t minMargin,
                uint16_t maxMargin,
                std::chrono::milliseconds aliveReferenceCycle,
                uint8_t failedReferenceCyclesTolerance) : ElementarySupervision(cSupervisionType),
                                                          mExpectedAliveIndicationsMin{minMargin > 0 && expectedAliveIndications > minMargin ? static_cast<uint16_t>(expectedAliveIndications - minMargin) : throw std::invalid_argument("The minimum margin should be greater than zero and smaller than the expected alive indications.")},
                                                          mExpectedAliveIndicationsMax{maxMargin > 0 ? static_cast<uint16_t>(expectedAliveIndications + maxMargin) : throw std::invalid_argument("The maximum margin should be greater than zero.")},
                                                          mFailedReferenceCyclesTolerance{failedReferenceCyclesTolerance > 0 ? failedReferenceCyclesTolerance : throw std::invalid_argument("The failed reference cycles tolerance should be greater than zero.")},
                                                          mAliveCounter{0},
                                                          mRunning{true},
                                                          mAliveIndications{0}
            {
                if (aliveReferenceCycle.count() > 0)
                {
                    mFuture =
                        std::async(
                            &AliveSupervision::supervise, this, aliveReferenceCycle);
                }
                else
                {
                    throw std::invalid_argument(
                        "The alive reference cycle should be greater than zero.");
                }
            }

            void AliveSupervision::supervise(
                std::chrono::milliseconds aliveReferenceCycle)
            {
                std::this_thread::sleep_for(aliveReferenceCycle);

                while (mRunning)
                {
                    if (mAliveIndications >= mExpectedAliveIndicationsMin &&
                        mAliveIndications <= mExpectedAliveIndicationsMax)
                    {
                        mAliveIndications = 0;

                        // Avoid wrapping
                        if (mAliveCounter > 0)
                        {
                            --mAliveCounter;
                        }

                        Report(SupervisionStatus::kOk);
                    }
                    else
                    {
                        mAliveIndications = 0;
                        ++mAliveCounter;

                        if (mAliveCounter < mFailedReferenceCyclesTolerance)
                        {
                            Report(SupervisionStatus::kFailed);
                        }
                        else
                        {
                            // If the supervision expired, it should be restarted.
                            mRunning = false;
                            Report(SupervisionStatus::kExpired);
                        }
                    }

                    std::this_thread::sleep_for(aliveReferenceCycle);
                }
            }

            void AliveSupervision::ReportAliveness() noexcept
            {
                // Avoid wrapping to zero
                if (mAliveIndications < std::numeric_limits<uint16_t>::max())
                {
                    ++mAliveIndications;
                }
            }

            AliveSupervision::~AliveSupervision()
            {
                mRunning = false;
                mFuture.wait();
            }
        }
    }
}