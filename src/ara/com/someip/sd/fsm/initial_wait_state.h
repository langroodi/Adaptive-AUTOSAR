#ifndef INITIAL_WAIT_STATE_H
#define INITIAL_WAIT_STATE_H

#include <random>
#include <thread>
#include <chrono>
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
                    /// @brief Server's or client's service initial wait state
                    /// @tparam T Server's or client state enumeration type
                    /// @note The state is not copyable
                    template <typename T>
                    class InitialWaitState : public TimerSetState<T>
                    {
                    private:
                        const int mInitialDelayMin;
                        const int mInitialDelayMax;

                    protected:
                        void SetTimer() override
                        {
                            // Generate a random initial delay
                            std::default_random_engine _generator;
                            std::uniform_int_distribution<int> _distribution(
                                mInitialDelayMin, mInitialDelayMax);
                            int _randomDely = _distribution(_generator);

                            // Sleep for the initali delay and then transit to the next state.
                            auto _delay = std::chrono::milliseconds(mInitialDelayMin);
                            std::this_thread::sleep_for(_delay);

                            if (!this->Stopped)
                            {
                                // Invoke the on timer expiration callback
                                this->OnTimerExpired();
                            }
                        }

                        void Deactivate(T nextState) override
                        {
                            // Nothing to do on deactivation
                        }

                    public:
                        /// @brief Constructor
                        /// @param currentState Current state at initial wait phase
                        /// @param nextState Next state after initial wait phase expiration
                        /// @param stoppedState Stopped state after put a stop to the service
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        /// @param initialDelayMin Minimum initial delay in milliseconds
                        /// @param initialDelayMax Maximum initial delay in milliseconds
                        InitialWaitState(
                            T currentState,
                            T nextState,
                            T stoppedState,
                            std::function<void()> onTimerExpired,
                            int initialDelayMin,
                            int initialDelayMax) noexcept : TimerSetState<T>(currentState, nextState, stoppedState, onTimerExpired),
                                                            mInitialDelayMin{initialDelayMin},
                                                            mInitialDelayMax{initialDelayMax}
                        {
                        }

                        InitialWaitState() = delete;
                        InitialWaitState(const InitialWaitState &) = delete;
                        InitialWaitState &operator=(const InitialWaitState &) = delete;
                    };
                }
            }
        }
    }
}

#endif