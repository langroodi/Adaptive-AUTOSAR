#ifndef NOTSUBSCRIBED_STATE_H
#define NOTSUBSCRIBED_STATE_H

#include "../../../helper/machine_state.h"

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
                    /// @brief Machine state in which the service is up but has no subscriber 
                    class NotSubscribedState : public helper::MachineState<helper::PubSubState>
                    {
                    protected:
                        void Deactivate(helper::PubSubState nextState) override;

                    public:
                        NotSubscribedState() noexcept;

                        NotSubscribedState(const NotSubscribedState &) = delete;
                        NotSubscribedState &operator=(const NotSubscribedState &) = delete;

                        void Activate(helper::PubSubState previousState) override;

                        /// @brief Notify that a new client has subscribed to the server
                        void Subscribed();

                        /// @brief Notify that the server has been stopped
                        void Stopped();
                    };
                }
            }
        }
    }
}

#endif