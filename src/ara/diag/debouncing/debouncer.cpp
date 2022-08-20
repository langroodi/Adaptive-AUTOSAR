#include "./debouncer.h"

namespace ara
{
    namespace diag
    {
        namespace debouncing
        {
            Debouncer::Debouncer(std::function<void(bool)> callback) : mEventStatus{EventStatus::kPending},
                                                                       onEventStatusChanged{callback}
            {
            }

            void Debouncer::SetEventStatus(EventStatus status)
            {
                if (mEventStatus != status)
                {
                    mEventStatus = status;

                    if (onEventStatusChanged)
                    {
                        switch (mEventStatus)
                        {
                        case EventStatus::kPassed:
                            onEventStatusChanged(true);
                            break;

                        case EventStatus::kFailed:
                            onEventStatusChanged(false);
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
}