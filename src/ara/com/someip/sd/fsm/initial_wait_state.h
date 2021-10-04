#ifndef INITIAL_WAIT_STATE_H
#define INITIAL_WAIT_STATE_H

#include <future>
#include <stdexcept>
#include <random>
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
                    /// @brief Server's or client's service initial wait state
                    /// @tparam T Server's or client state enumeration type
                    /// @note The state is not copyable
                    template <typename T>
                    class InitialWaitState : public helper::MachineState<T>
                    {
                    private:
                        const T mNextState;
                        const int mInitialDelayMin;
                        const int mInitialDelayMax;
                        const std::function<void()> mOnTimerExpired;
                        std::future<void> mFuture;

                        void setTimer(int initialDelay)
                        {
                            // Sleep for the initali delay and then transit to the next state.
                            auto _delay = std::chrono::milliseconds(initialDelay);
                            std::this_thread::sleep_for(_delay);

                            // Invoke the on timer expiration callback
                            mOnTimerExpired();

                            helper::MachineState<T>::Transit(mNextState);
                            // Make the future invalid.
                            mFuture.get();
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
                        /// @param initialDelayMin Minimum initial delay in milliseconds
                        /// @param initialDelayMax Maximum initial delay in milliseconds
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        InitialWaitState(
                            T currentState,
                            T nextState,
                            int initialDelayMin,
                            int initialDelayMax,
                            std::function<void()> onTimerExpired) noexcept : helper::MachineState<T>(currentState),
                                                                             mNextState{nextState},
                                                                             mInitialDelayMin{initialDelayMin},
                                                                             mInitialDelayMax{initialDelayMax},
                                                                             mOnTimerExpired{onTimerExpired}
                        {
                        }

                        InitialWaitState(const InitialWaitState &) = delete;
                        InitialWaitState &operator=(const InitialWaitState &) = delete;

                        void Activate(T previousState) override
                        {
                            // Valid future means the timer is not expired yet.
                            if (mFuture.valid())
                            {
                                throw std::logic_error(
                                    "The state has been already activated");
                            }
                            else
                            {
                                std::default_random_engine _generator;
                                std::uniform_int_distribution<int> _distribution(
                                    mInitialDelayMin, mInitialDelayMax);
                                int _initialDelay = _distribution(_generator);

                                // Set the timer from a new thread with a random initial delay.
                                mFuture =
                                    std::async(
                                        std::launch::async,
                                        &InitialWaitState<T>::setTimer,
                                        this,
                                        _initialDelay);
                            }
                        }
                    };
                }
            }
        }
    }
}

#endif