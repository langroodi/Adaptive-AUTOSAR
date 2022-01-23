#include "client_repetition_state.h"

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
                    ClientRepetitionState::ClientRepetitionState(
                        helper::TtlTimer *ttlTimer,
                        std::function<void()> onTimerExpired,
                        uint32_t repetitionsMax,
                        int repetitionsBaseDelay) : MachineState<helper::SdClientState>(helper::SdClientState::RepetitionPhase),
                                                    ClientServiceState(ttlTimer),
                                                    RepetitionState<helper::SdClientState>(helper::SdClientState::RepetitionPhase,
                                                                                           helper::SdClientState::Stopped,
                                                                                           helper::SdClientState::Stopped,
                                                                                           onTimerExpired,
                                                                                           repetitionsMax,
                                                                                           repetitionsBaseDelay)
                    {
                    }

                    void ClientRepetitionState::Activate(helper::SdClientState previousState)
                    {
                        // Reset the next state in case that it was modified during the last service offering.
                        SetNextState(helper::SdClientState::Stopped);
                        TimerSetState::Activate(previousState);
                    }
                    
                    void ClientRepetitionState::ServiceOffered(uint32_t ttl) noexcept
                    {
                        // Instead of going to stopped state after interruption, transit to the service ready state.
                        SetNextState(helper::SdClientState::ServiceReady);
                        Timer->Set(ttl);
                        Interrupt();
                    }
                }
            }
        }
    }
}
