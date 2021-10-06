#include "./service_ready_state.h"

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
                    ServiceReadyState::ServiceReadyState(
                        helper::TtlTimer *ttlTimer) noexcept : ClientServiceState(SdClientState::ServiceReady,
                                                                                  ttlTimer)

                    {
                    }

                    void ServiceReadyState::onTimerExpired()
                    {
                        Transit(SdClientState::InitialWaitPhase);
                    }

                    void ServiceReadyState::Activate(SdClientState previousState)
                    {
                        auto _callback =
                            std::bind(&ServiceReadyState::onTimerExpired, this);
                        Timer->SetExpirationCallback(_callback);
                    }

                    void ServiceReadyState::ServiceNotRequested()
                    {
                        Transit(SdClientState::ServiceSeen);
                    }

                    void ServiceReadyState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                    }

                    void ServiceReadyState::ServiceStopped()
                    {
                        Timer->Cancel();
                        Transit(SdClientState::Stopped);
                    }

                    void ServiceReadyState::Deactivate(SdClientState nextState)
                    {
                        Timer->ResetExpirationCallback();
                    }

                    ServiceReadyState::~ServiceReadyState()
                    {
                        Timer->ResetExpirationCallback();
                    }
                }
            }
        }
    }
}