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
                        std::function<void()> onTimerExpired,
                        uint32_t repetitionsMax,
                        int repetitionsBaseDelay) : MachineState<helper::SdClientState>(helper::SdClientState::RepetitionPhase),
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
                    }
                    
                    void ClientRepetitionState::ServiceOffered(uint32_t ttl)
                    {
                        // Instead of going to stopped state after interruption, transit to the service ready state.
                        SetNextState(helper::SdClientState::ServiceReady);
                        Interrupt();
                    }
                }
            }
        }
    }
}
