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
                                                    helper::SdServerState::MainPhase,
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
                            std::this_thread::sleep_for(mCyclicOfferDelay);

                            if (this->Stopped)
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

                    void MainState::Deactivate(helper::SdServerState nextState)
                    {
                        // Nothing to do on deactivation
                    }
                }
            }
        }
    }
}