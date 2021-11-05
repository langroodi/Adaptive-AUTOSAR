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
                    public:
                        ClientServiceState() noexcept = default;
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