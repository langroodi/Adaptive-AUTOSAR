#ifndef STOPPED_STATE_H
#define STOPPED_STATE_H

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
                    /// @brief Client state when the service is stopped
                    /// @note The state is not copyable
                    class StoppedState : public ClientServiceState
                    {
                    protected:
                        void Deactivate(helper::SdClientState nextState) override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        StoppedState(helper::TtlTimer *ttlTimer) noexcept;

                        StoppedState() = delete;
                        StoppedState(const StoppedState &) = delete;
                        StoppedState &operator=(const StoppedState &) = delete;

                        void Activate(helper::SdClientState previousState) override;

                        /// @brief Inform the state that the client's service is not requested anymore
                        void ServiceNotRequested();

                        void ServiceOffered(uint32_t ttl) override;
                    };
                }
            }
        }
    }
}

#endif