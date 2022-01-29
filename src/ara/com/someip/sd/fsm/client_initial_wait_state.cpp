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

                    void ClientInitialWaitState::SetTimer()
                    {
                        // Generate a random initial delay
                        std::default_random_engine _generator;
                        std::uniform_int_distribution<int> _distribution(
                            this->InitialDelayMin, this->InitialDelayMax);
                        int _randomDely = _distribution(_generator);

                        // Sleep for the initali random delay and
                        // then transit to the next state
                        auto _delay = std::chrono::milliseconds(_randomDely);
                        bool _interrupted = this->WaitFor(_delay);

                        if (Timer->GetOffered())
                        {
                            SetNextState(helper::SdClientState::ServiceReady);
                        }
                        else if (!_interrupted)
                        {
                            // Invoke the on timer expiration callback
                            this->OnTimerExpired();
                        }
                    }
                }
            }
        }
    }
}