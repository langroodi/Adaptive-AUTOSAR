#include "./counter_based_debouncer.h"

namespace ara
{
    namespace diag
    {
        namespace debouncing
        {
            CounterBasedDebouncer::CounterBasedDebouncer(
                std::function<void(bool)> callback,
                const CounterBased &defaultValues) : Debouncer(callback),
                                                     mDefaultValues{defaultValues},
                                                     mEventStatus{EventStatus::kPending},
                                                     mFdc{0}
            {
            }

            void CounterBasedDebouncer::SetEventStatus(EventStatus status)
            {
                if (mEventStatus != status)
                {
                    mEventStatus = status;

                    if (OnEventStatusChanged)
                    {
                        switch (mEventStatus)
                        {
                        case EventStatus::kPassed:
                            OnEventStatusChanged(true);
                            break;

                        case EventStatus::kFailed:
                            OnEventStatusChanged(false);
                            break;

                        default:
                            break;
                        }
                    }
                }
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

            void CounterBasedDebouncer::Freeze()
            {
                Reset();
            }

            void CounterBasedDebouncer::Reset()
            {
                mFdc = 0;
                mEventStatus = EventStatus::kPending;
            }
        }
    }
}