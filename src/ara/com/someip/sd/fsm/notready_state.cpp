#include "./notready_state.h"

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
                    NotReadyState::NotReadyState() : helper::MachineState<SdServerState>(SdServerState::NotReady)
                    {
                    }

                    void NotReadyState::Activate(SdServerState previousState)
                    {
                        // Nothing to do on activation.
                    }

                    void NotReadyState::ServiceActivated()
                    {
                        Transit(SdServerState::InitialWaitPhase);
                    }

                    void NotReadyState::Deactivate(SdServerState nextState)
                    {
                        // Nothing to do on deactivation.
                    }
                }
            }
        }
    }
}