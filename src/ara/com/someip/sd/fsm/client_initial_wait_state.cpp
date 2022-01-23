#include "client_initial_wait_state.h"

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
                    ClientInitialWaitState::ClientInitialWaitState(
                        helper::TtlTimer *ttlTimer,
                        std::function<void()> onTimerExpired,
                        int initialDelayMin,
                        int initialDelayMax) : MachineState<helper::SdClientState>(helper::SdClientState::InitialWaitPhase),
                                               ClientServiceState(ttlTimer),
                                               InitialWaitState(helper::SdClientState::InitialWaitPhase,
                                                                helper::SdClientState::RepetitionPhase,
                                                                helper::SdClientState::Stopped,
                                                                onTimerExpired,
                                                                initialDelayMin,
                                                                initialDelayMax)
                    {
                    }

                    void ClientInitialWaitState::Activate(helper::SdClientState previousState)
                    {
                        // Reset the next state in case that it was modified during the last service offering.
                        SetNextState(helper::SdClientState::RepetitionPhase);
                        TimerSetState::Activate(previousState);
                    }

                    void ClientInitialWaitState::ServiceOffered(uint32_t ttl) noexcept
                    {
                        // Instead of going to repetition state after interruption, transit to the service ready state.
                        SetNextState(helper::SdClientState::ServiceReady);
                        Timer->Set(ttl);
                        Interrupt();
                    }
                }
            }
        }
    }
}