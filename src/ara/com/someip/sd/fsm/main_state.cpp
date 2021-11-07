#include <thread>
#include <chrono>
#include "./main_state.h"

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
                    MainState::MainState(
                        std::function<void()> onTimerExpired,
                        int cyclicOfferDelay) : helper::MachineState<helper::SdServerState>(helper::SdServerState::MainPhase),
                                                TimerSetState<helper::SdServerState>(
                                                    helper::SdServerState::NotReady,
                                                    helper::SdServerState::NotReady,
                                                    onTimerExpired),
                                                mCyclicOfferDelay{cyclicOfferDelay}
                    {
                        if (cyclicOfferDelay < 0)
                        {
                            throw std::invalid_argument("Invalid cyclic offer delay.");
                        }
                    }

                    void MainState::SetTimer()
                    {
                        while (true)
                        {
                            bool _interrupted = this->WaitFor(mCyclicOfferDelay);

                            if (_interrupted)
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