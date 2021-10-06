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
                        helper::TtlTimer *ttlTimer) noexcept : ClientServiceState(SdClientState::ServiceSeen,
                                                                                  ttlTimer)

                    {
                    }

                    void ServiceSeenState::onTimerExpired()
                    {
                        Transit(SdClientState::ServiceNotSeen);
                    }

                    void ServiceSeenState::Activate(SdClientState previousState)
                    {
                        auto _callback =
                            std::bind(&ServiceSeenState::onTimerExpired, this);
                        Timer->SetExpirationCallback(_callback);
                    }

                    void ServiceSeenState::ServiceRequested()
                    {
                        Transit(SdClientState::ServiceReady);
                    }

                    void ServiceSeenState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                    }

                    void ServiceSeenState::ServiceStopped()
                    {
                        Timer->Cancel();
                        Transit(SdClientState::ServiceNotSeen);
                    }

                    void ServiceSeenState::Deactivate(SdClientState nextState)
                    {
                        Timer->ResetExpirationCallback();
                    }

                    ServiceSeenState::~ServiceSeenState()
                    {
                        Timer->ResetExpirationCallback();
                    }
                }
            }
        }
    }
}