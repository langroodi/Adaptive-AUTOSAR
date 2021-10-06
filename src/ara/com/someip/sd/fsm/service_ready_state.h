#ifndef SERVICE_READY_STATE_H
#define SERVICE_READY_STATE_H

#include "./client_service_state.h"

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
                    class ServiceReadyState : public ClientServiceState
                    {
                    private:
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

                        void ServiceOffered(uint32_t ttl) override;

                        /// @brief Inform the state that the service has been stopped
                        void ServiceStopped();
                    };
                }
            }
        }
    }
}

#endif