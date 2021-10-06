#include "./stopped_state.h"

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
                    StoppedState::StoppedState(
                        helper::TtlTimer *ttlTimer) noexcept : ClientServiceState(SdClientState::Stopped,
                                                                                  ttlTimer)
                    {
                    }

                    void StoppedState::Activate(SdClientState previousState)
                    {
                        // Nothing to do on activation
                    }

                    void StoppedState::ServiceNotRequested()
                    {
                        Transit(SdClientState::ServiceNotSeen);
                    }

                    void StoppedState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                        Transit(SdClientState::ServiceReady);
                    }

                    void StoppedState::Deactivate(SdClientState nextState)
                    {
                        // Nothig to do on deactivation
                    }
                };
            }
        }
    }
}