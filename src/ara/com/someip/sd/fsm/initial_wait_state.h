#ifndef INITIAL_WAIT_STATE_H
#define INITIAL_WAIT_STATE_H

#include <random>
#include <future>
#include <thread>
#include <chrono>
#include "../../../helper/machine_state.h"

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
                    /// @brief Server's or client's service initial state
                    /// @tparam T Server's or client state enumeration type
                    /// @note The state is not copyable
                    template <typename T>
                    class InitialWaitState : public helper::MachineState<T>
                    {
                    private:
                        const T mNextState;
                        const int mInitialDelayMin;
                        const int mInitialDelayMax;

                        void setTimer(int initialDelay)
                        {
                            // Sleep for the initali delay and then transit to the next state.
                            auto _delay = std::chrono::seconds(initialDelay);
                            std::this_thread::sleep_for(_delay);
                            Transit(mNextState);
                        }

                    protected:
                        void Deactivate(T nextState) override
                        {
                            // Nothing to do on deactivation.
                        }

                    public:
                        /// @brief Constructor
                        /// @param currentState Current state at initial wait phase
                        /// @param nextState Next state after initial wait phase expiration
                        /// @param initialDelayMin Minimum initial delay in seconds
                        /// @param initialDelayMax Maximum initial delay in seconds
                        InitialWaitState(
                            T currentState,
                            T nextState,
                            int initialDelyMin,
                            int initialDelayMax) : helper::MachineState<T>(currentState),
                                                   mNextState{nextState},
                                                   mInitialDelayMin{initialDelyMin},
                                                   mInitialDelayMax{initialDelayMax}
                        {
                        }

                        InitialWaitState(const InitialWaitState &) = delete;
                        InitialWaitState &operator=(const InitialWaitState &) = delete;

                        void Activate(T previousState) override
                        {
                            std::default_random_engine _generator;
                            std::uniform_real_distribution<int> _distribution(
                                mInitialDelayMin, mInitialDelayMax);
                            int _initialDelay = _distribution(_generator);

                            // Set the timer from a new thread with a random initial delay.
                            std::async(std::launch::async, setTimer, _initialDelay);
                        }
                    };
                }
            }
        }
    }
}

#endif