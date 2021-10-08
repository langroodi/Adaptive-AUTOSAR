#ifndef CLIENT_SERVICE_STATE_H
#define CLIENT_SERVICE_STATE_H

#include "../../../helper/machine_state.h"
#include "../../../helper/ttl_timer.h"

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
                    /// @brief Abstract client's service state
                    /// @details The class forces its children to react on service offering.
                    /// @note The state is not copyable
                    class ClientServiceState : public helper::MachineState<helper::SdClientState>
                    {
                    protected:
                        /// @brief Timer to handle service offer entry TTL
                        helper::TtlTimer *const Timer;

                    public:
                        /// @brief Constructor
                        /// @param state Current client state
                        /// @param timer Finite machine state global TTL timer pointer
                        ClientServiceState(
                            helper::SdClientState state,
                            helper::TtlTimer *timer) : helper::MachineState<helper::SdClientState>(state),
                                                       Timer{timer}
                        {
                        }

                        ClientServiceState(const ClientServiceState &) = delete;
                        ClientServiceState &operator=(const ClientServiceState &) = delete;
                        virtual ~ClientServiceState() override = default;

                        /// @brief Inform the state that the service is offered
                        /// @param ttl Received service offer entry TTL
                        virtual void ServiceOffered(uint32_t ttl) = 0;
                    };
                }
            }
        }
    }
}

#endif