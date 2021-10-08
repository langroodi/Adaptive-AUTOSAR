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
                        helper::TtlTimer *ttlTimer) noexcept : ClientServiceState(helper::SdClientState::ServiceNotSeen,
                                                                                  ttlTimer)
                    {
                    }

                    void ServiceNotseenState::Activate(helper::SdClientState previousState)
                    {
                        // Nothing to do on activation
                    }

                    void ServiceNotseenState::ServiceRequested()
                    {
                        Transit(helper::SdClientState::InitialWaitPhase);
                    }

                    void ServiceNotseenState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Set(ttl);
                        Transit(helper::SdClientState::ServiceSeen);
                    }

                    void ServiceNotseenState::Deactivate(helper::SdClientState nextState)
                    {
                        // Nothing to do on deactivation
                    }
                }
            }
        }
    }
}