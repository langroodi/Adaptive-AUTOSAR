#include "./notsubscribed_state.h"

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
                    NotSubscribedState::NotSubscribedState() noexcept : helper::MachineState<helper::PubSubState>(helper::PubSubState::NotSubscribed)
                    {
                    }

                    void NotSubscribedState::Activate(helper::PubSubState previousState)
                    {
                        // Nothing to do on activation
                    }

                    void NotSubscribedState::Subscribed()
                    {
                        Transit(helper::PubSubState::Subscribed);
                    }

                    void NotSubscribedState::Stopped()
                    {
                        Transit(helper::PubSubState::ServiceDown);
                    }

                    void NotSubscribedState::Deactivate(helper::PubSubState nextState)
                    {
                        // Nothing to do on deactivation
                    }
                }
            }
        }
    }
}