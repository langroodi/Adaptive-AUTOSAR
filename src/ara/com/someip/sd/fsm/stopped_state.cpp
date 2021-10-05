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
                    StoppedState::StoppedState() noexcept : helper::MachineState<SdClientState>(
                                                                SdClientState::Stopped)
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

                    void StoppedState::Deactivate(SdClientState nextState)
                    {
                        // Nothig to do on deactivation
                    }
                };
            }
        }
    }
}