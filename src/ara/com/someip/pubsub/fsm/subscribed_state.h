#ifndef SUBSCRIBED_STATE_H
#define SUBSCRIBED_STATE_H

#include <cstdint>
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
                    /// @brief Machine state in which the service is up and it has at least a subscriber
                    class SubscribedState : public helper::MachineState<helper::PubSubState>
                    {
                    private:
                        int32_t mSubscriptionCounter;

                    protected:
                        void Deactivate(helper::PubSubState nextState) override;

                    public:
                        /// @brief Default constructor
                        SubscribedState() noexcept;

                        SubscribedState(const SubscribedState &) = delete;
                        SubscribedState &operator=(const SubscribedState &) = delete;

                        void Activate(helper::PubSubState previousState) override;

                        /// @brief Notify that a new client has subscribed to the server
                        void Subscribed() noexcept;

                        /// @brief Notify that a new client has unsubscribed from the server
                        void Unsubscribed();

                        /// @brief Notify that the server has been stopped
                        void Stopped();
                    };
                }
            }
        }
    }
}

#endif