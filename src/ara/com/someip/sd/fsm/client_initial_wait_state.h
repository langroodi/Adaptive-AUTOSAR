#ifndef CLIENT_INITIAL_WAIT_STATE_H
#define CLIENT_INITIAL_WAIT_STATE_H

#include "./client_service_state.h"
#include "./initial_wait_state.h"

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
                    /// @brief Client's service initial wait state
                    class ClientInitialWaitState : public ClientServiceState, public InitialWaitState<helper::SdClientState>
                    {
                    protected:
                        void Activate(helper::SdClientState previousState) override;
                        void SetTimer() override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        /// @param initialDelayMin Minimum initial delay in milliseconds
                        /// @param initialDelayMax Maximum initial delay in milliseconds
                        ClientInitialWaitState(
                            helper::TtlTimer *ttlTimer,
                            std::function<void()> onTimerExpired,
                            int initialDelayMin,
                            int initialDelayMax);

                        ClientInitialWaitState() = delete;
                        ClientInitialWaitState(const ClientInitialWaitState &) = delete;
                        ClientInitialWaitState &operator=(const ClientInitialWaitState &) = delete;
                    };
                }
            }
        }
    }
}

#endif