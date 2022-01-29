#ifndef CLIENT_REPETITION_STATE_H
#define CLIENT_REPETITION_STATE_H

#include "./client_service_state.h"
#include "./repetition_state.h"

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
                    /// @brief Client's service repetition state
                    class ClientRepetitionState : public ClientServiceState, public RepetitionState<helper::SdClientState>
                    {
                    protected:
                        void Activate(helper::SdClientState previousState) override;
                        void SetTimer() override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        /// @param repetitionsMax Maximum iteration in repetition phase
                        /// @param repetitionsBaseDelay Repetition iteration delay in milliseconds
                        ClientRepetitionState(
                            helper::TtlTimer *ttlTimer,
                            std::function<void()> onTimerExpired,
                            uint32_t repetitionsMax,
                            int repetitionsBaseDelay);

                        ClientRepetitionState() = delete;
                        ClientRepetitionState(const ClientRepetitionState &) = delete;
                        ClientRepetitionState &operator=(const ClientRepetitionState &) = delete;
                    };
                }
            }
        }
    }
}

#endif