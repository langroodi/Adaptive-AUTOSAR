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
                        helper::TtlTimer *ttlTimer) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::Stopped),
                        mTimer{ttlTimer}
                    {
                    }

                    void StoppedState::Activate(helper::SdClientState previousState)
                    {
                        // Nothing to do on activation
                    }

                    void StoppedState::ServiceNotRequested()
                    {
                        Transit(helper::SdClientState::ServiceNotSeen);
                    }

                    void StoppedState::ServiceOffered(uint32_t ttl)
                    {
                        mTimer->Reset(ttl);
                        Transit(helper::SdClientState::ServiceReady);
                    }

                    void StoppedState::Deactivate(helper::SdClientState nextState)
                    {
                        // Nothig to do on deactivation
                    }
                };
            }
        }
    }
}