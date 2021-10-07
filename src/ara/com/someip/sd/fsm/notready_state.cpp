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
                    NotReadyState::NotReadyState() noexcept : helper::MachineState<helper::SdServerState>(
                                                                  helper::SdServerState::NotReady)
                    {
                    }

                    void NotReadyState::Activate(helper::SdServerState previousState)
                    {
                        // Nothing to do on activation.
                    }

                    void NotReadyState::ServiceActivated()
                    {
                        Transit(helper::SdServerState::InitialWaitPhase);
                    }

                    void NotReadyState::Deactivate(helper::SdServerState nextState)
                    {
                        // Nothing to do on deactivation.
                    }
                }
            }
        }
    }
}