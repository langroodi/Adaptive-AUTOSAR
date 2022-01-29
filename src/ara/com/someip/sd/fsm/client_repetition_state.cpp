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

                    void ClientRepetitionState::SetTimer()
                    {
                        for (uint32_t i = 0; i < this->RepetitionsMax; ++i)
                        {
                            int _doubledDelay = std::pow(2, i) * this->RepetitionsBaseDelay;
                            auto _delay = std::chrono::milliseconds(_doubledDelay);
                            bool _interrupted = this->WaitFor(_delay);

                            if (Timer->GetOffered())
                            {
                                SetNextState(helper::SdClientState::ServiceReady);
                                break;
                            }
                            else if (_interrupted)
                            {
                                break;
                            }
                            else
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
}
