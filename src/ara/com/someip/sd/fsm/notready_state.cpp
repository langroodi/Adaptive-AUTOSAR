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
                    NotReadyState::NotReadyState(std::function<void()> onServiceStopped) : helper::MachineState<helper::SdServerState>(
                                                                                               helper::SdServerState::NotReady),
                                                                                           mOnServiceStopped(onServiceStopped)
                    {
                    }

                    void NotReadyState::Activate(helper::SdServerState previousState)
                    {
                        // If the state activate not as the entrypoint, it means the service has been stopped.
                        // At entrypoint the current state and the previous state are the same.
                        if (previousState != helper::SdServerState::NotReady)
                        {
                            mOnServiceStopped();
                        }
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