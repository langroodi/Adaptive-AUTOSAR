#ifndef SERVICE_NOTSEEN_STATE_H
#define SERVICE_NOTSEEN_STATE_H

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
                    /// @brief Client's service state when the service is not seen
                    /// @details The state happens when the client's service is not requested and also the server's service is down.
                    /// @note The state is not copyable
                    class ServiceNotseenState : public ClientServiceState
                    {
                    private:
                        helper::TtlTimer *mTimer;

                    protected:
                        void Deactivate(helper::SdClientState nextState) override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        ServiceNotseenState(helper::TtlTimer *ttlTimer) noexcept;

                        ServiceNotseenState() = delete;
                        ServiceNotseenState(const ServiceNotseenState &) = delete;
                        ServiceNotseenState &operator=(const ServiceNotseenState &) = delete;

                        void Activate(helper::SdClientState previousState) override;

                        /// @brief Inform the state that the client's service is requested
                        void ServiceRequested();

                        void ServiceOffered(uint32_t ttl) override;
                    };
                }
            }
        }
    }
}

#endif