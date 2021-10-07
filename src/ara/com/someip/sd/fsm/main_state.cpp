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
                        int cyclicOfferDelay) : TimerSetState<helper::SdServerState>(helper::SdServerState::MainPhase,
                                                                             helper::SdServerState::MainPhase,
                                                                             helper::SdServerState::NotReady,
                                                                             onTimerExpired),
                                                mCyclicOfferDelay{cyclicOfferDelay}
                    {
                    }

                    void MainState::SetTimer()
                    {
                        while (!this->Stopped)
                        {
                            std::this_thread::sleep_for(mCyclicOfferDelay);

                            // Invoke the on timer expiration callback
                            this->OnTimerExpired();
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