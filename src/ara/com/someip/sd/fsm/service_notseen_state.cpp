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
                    ServiceNotseenState::ServiceNotseenState() noexcept : helper::MachineState<SdClientState>(
                                                                              SdClientState::ServiceNotSeen)
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
                    
                    void ServiceNotseenState::ServiceOffered()
                    {
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