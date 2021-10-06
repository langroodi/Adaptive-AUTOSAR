#include "./service_notseen_state.h"

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
                    ServiceNotseenState::ServiceNotseenState(
                        helper::TtlTimer *ttlTimer) noexcept : ClientServiceState(SdClientState::ServiceNotSeen,
                                                                                  ttlTimer)
                    {
                    }

                    void ServiceNotseenState::Activate(SdClientState previousState)
                    {
                        // Nothing to do on activation
                    }

                    void ServiceNotseenState::ServiceRequested()
                    {
                        Transit(SdClientState::InitialWaitPhase);
                    }

                    void ServiceNotseenState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Set(ttl);
                        Transit(SdClientState::ServiceSeen);
                    }

                    void ServiceNotseenState::Deactivate(SdClientState nextState)
                    {
                        // Nothing to do on deactivation
                    }
                }
            }
        }
    }
}