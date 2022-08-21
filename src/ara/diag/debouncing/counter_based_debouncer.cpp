#include "./counter_based_debouncer.h"

namespace ara
{
    namespace diag
    {
        namespace debouncing
        {
            CounterBasedDebouncer::CounterBasedDebouncer(
                std::function<void(bool)> callback,
                CounterBased defaultValues) : Debouncer(callback),
                                              mDefaultValues{defaultValues},
                                              mFdc{0}
            {
            }

            void CounterBasedDebouncer::ReportPrepassed()
            {
                if ((mFdc - mDefaultValues.passedStepsize) <= mDefaultValues.passedThreshold)
                {
                    mFdc = mDefaultValues.passedThreshold;
                    SetEventStatus(EventStatus::kPassed);
                }
                else if (mDefaultValues.useJumpToPassed && mFdc > mDefaultValues.passedJumpValue)
                {
                    mFdc = mDefaultValues.passedJumpValue;
                }
                else
                {
                    mFdc -= mDefaultValues.passedStepsize;
                }
            }

            void CounterBasedDebouncer::ReportPassed()
            {
                mFdc = mDefaultValues.passedThreshold;
                SetEventStatus(EventStatus::kPassed);
            }

            void CounterBasedDebouncer::ReportPrefailed()
            {
                if ((mFdc + mDefaultValues.failedStepsize) >= mDefaultValues.failedThreshold)
                {
                    mFdc = mDefaultValues.failedThreshold;
                    SetEventStatus(EventStatus::kFailed);
                }
                else if (mDefaultValues.useJumpToFailed && mFdc < mDefaultValues.failedJumpValue)
                {
                    mFdc = mDefaultValues.failedJumpValue;
                }
                else
                {
                    mFdc += mDefaultValues.failedStepsize;
                }
            }

            void CounterBasedDebouncer::ReportFailed()
            {
                mFdc = mDefaultValues.failedThreshold;
                SetEventStatus(EventStatus::kFailed);
            }

            void CounterBasedDebouncer::Freeze()
            {
                Reset();
            }

            void CounterBasedDebouncer::Reset()
            {
                mFdc = 0;
                SetEventStatus(EventStatus::kPending);
            }
        }
    }
}