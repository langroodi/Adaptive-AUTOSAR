#ifndef SERVICE_NOTSEEN_STATE_H
#define SERVICE_NOTSEEN_STATE_H

#include "../../../helper/machine_state.h"
#include "../someip_sd_client.h"

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
                    /// @brief Client's service state when the service is not seen
                    /// @details The state happens when the client's service is not requested and also the server's service is down.
                    /// @note The state is not copyable
                    class ServiceNotseenState : public helper::MachineState<SdClientState>
                    {
                    protected:
                        void Deactivate(SdClientState nextState) override;

                    public:
                        ServiceNotseenState() noexcept;

                        ServiceNotseenState(const ServiceNotseenState &) = delete;
                        ServiceNotseenState &operator=(const ServiceNotseenState &) = delete;

                        void Activate(SdClientState previousState) override;

                        /// @brief Inform the state that the client's service is requested
                        void ServiceRequested();

                        /// @brief Inform the state that the service has been offered
                        void ServiceOffered();
                    };
                }
            }
        }
    }
}

#endif