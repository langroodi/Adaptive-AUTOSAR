#ifndef SERVICE_READY_STATE_H
#define SERVICE_READY_STATE_H

#include "../../../helper/machine_state.h"
#include "../../../helper/ttl_timer.h"
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
                    /// @brief Client's state when the service is requested and also up
                    /// @note The state is not copyable.
                    class ServiceReadyState : public helper::MachineState<SdClientState>
                    {
                    private:
                        helper::TtlTimer *const mTtlTimer;
                        void onTimerExpired();

                    protected:
                        void Deactivate(SdClientState nextState) override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        ServiceReadyState(helper::TtlTimer *ttlTimer) noexcept;

                        ServiceReadyState() = delete;
                        ServiceReadyState(const ServiceReadyState &) = delete;
                        ServiceReadyState &operator=(const ServiceReadyState &) = delete;
                        ~ServiceReadyState() override;

                        void Activate(SdClientState previousState) override;

                        /// @brief Inform the state that the client's service is not requested anymore
                        void ServiceNotRequested();

                        /// @brief Inform the state that the service has been stopped
                        void ServiceStopped();
                    };
                }
            }
        }
    }
}

#endif