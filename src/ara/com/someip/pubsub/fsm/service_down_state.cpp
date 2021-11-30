#include "./service_down_state.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                namespace fsm
                {
                    ServiceDownState::ServiceDownState() noexcept : helper::MachineState<helper::PubSubState>(helper::PubSubState::ServiceDown)
                    {
                    }

                    void ServiceDownState::Activate(helper::PubSubState previousState)
                    {
                        // Nothing to do on activation
                    }

                    void ServiceDownState::Started()
                    {
                        Transit(helper::PubSubState::NotSubscribed);
                    }

                    void ServiceDownState::Deactivate(helper::PubSubState nextState)
                    {
                        // Nothing to do on deactivation
                    }
                }
            }
        }
    }
}