#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "./timer_set_state.h"

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
                    /// @brief Server's main state
                    /// @note The state is not copyable
                    class MainState : public TimerSetState<helper::SdServerState>
                    {
                    private:
                        const std::chrono::milliseconds mCyclicOfferDelay;

                    protected:
                        void SetTimer() override;

                    public:
                        /// @brief Constructor
                        /// @param cyclicOfferDelay Interval in milliseconds to offer the service
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        MainState(
                            std::function<void()> onTimerExpired,
                            int cyclicOfferDelay);

                        MainState() = delete;
                        MainState(const MainState &) = delete;
                        MainState &operator=(const MainState &) = delete;
                    };
                }
            }
        }
    }
}

#endif