#include "./service_seen_state.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                namespace fsm
                {
                    ServiceSeenState::ServiceSeenState(
                        helper::TtlTimer *ttlTimer) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::ServiceSeen),
                                                               mTimer{ttlTimer}
                    {
                    }

                    void ServiceSeenState::onTimerExpired()
                    {
                        Transit(helper::SdClientState::ServiceNotSeen);
                    }

                    void ServiceSeenState::Activate(helper::SdClientState previousState)
                    {
                        auto _callback =
                            std::bind(&ServiceSeenState::onTimerExpired, this);
                        mTimer->SetExpirationCallback(_callback);
                    }

                    void ServiceSeenState::ServiceRequested()
                    {
                        Transit(helper::SdClientState::ServiceReady);
                    }

                    void ServiceSeenState::ServiceOffered(uint32_t ttl)
                    {
                        mTimer->Reset(ttl);
                    }

                    void ServiceSeenState::ServiceStopped()
                    {
                        mTimer->Cancel();
                        Transit(helper::SdClientState::ServiceNotSeen);
                    }

                    void ServiceSeenState::Deactivate(helper::SdClientState nextState)
                    {
                        mTimer->ResetExpirationCallback();
                    }

                    ServiceSeenState::~ServiceSeenState()
                    {
                        mTimer->ResetExpirationCallback();
                    }
                }
            }
        }
    }
}