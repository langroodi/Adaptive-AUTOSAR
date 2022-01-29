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
                    class ClientServiceState : virtual public helper::MachineState<helper::SdClientState>
                    {
                    protected:
                        /// @brief Timer to handle service offer entry TTL
                        helper::TtlTimer *const Timer;

                        /// @brief Constructor
                        /// @param timer Finite machine state global TTL timer pointer
                        ClientServiceState(helper::TtlTimer *timer) : Timer{timer}
                        {
                        }

                    public:
                        ClientServiceState(const ClientServiceState &) = delete;
                        ClientServiceState &operator=(const ClientServiceState &) = delete;
                        virtual ~ClientServiceState() noexcept = default;
                    };
                }
            }
        }
    }
}

#endif