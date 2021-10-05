#ifndef TIMER_SET_STATE_H
#define TIMER_SET_STATE_H

#include <future>
#include <stdexcept>
#include <random>
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
                    /// @brief Server's or client's service timer set state
                    /// @tparam T Server's or client state enumeration type
                    /// @note The state is not copyable
                    template <typename T>
                    class TimerSetState : public helper::MachineState<T>
                    {
                    private:
                        const T mNextState;
                        const T mStoppedState;
                        std::future<void> mFuture;

                        void setTimerBase()
                        {
                            SetTimer();

                            helper::MachineState<T>::Transit(mNextState);
                            // Make the future invalid if it is already valid.
                            if (mFuture.valid())
                            {
                                mFuture.get();
                            }
                        }

                    protected:
                        /// @brief Inidicates whether the server's service stopped or not
                        bool Stopped;

                        /// @brief Delegate which is invoked by timer's thread when the timer is expired
                        const std::function<void()> OnTimerExpired;

                        /// @brief Set the phase time on state activation
                        virtual void SetTimer() = 0;

                        /// @brief Get a random number
                        /// @param lowerBound Random number lower bound
                        /// @param upperBound Random number upper bound
                        /// @returns Random number within the determined bound
                        static int GetRandom(int lowerBound, int upperBound)
                        {
                            std::default_random_engine _generator;
                            std::uniform_int_distribution<int> _distribution(
                                lowerBound, upperBound);
                            int _result = _distribution(_generator);

                            return _result;
                        }

                        void Deactivate(T nextState) override
                        {
                            // If the future is valid, wait until it becomes invalid (the timer expires).
                            if (mFuture.valid())
                            {
                                mFuture.wait();
                            }
                        }

                    public:
                        /// @brief Constructor
                        /// @param currentState Current state at initial wait phase
                        /// @param nextState Next state after initial wait phase expiration
                        /// @param stoppedState Stopped state after put a stop to the service
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        TimerSetState(
                            T currentState,
                            T nextState,
                            T stoppedState,
                            std::function<void()> onTimerExpired) noexcept : helper::MachineState<T>(currentState),
                                                                             mNextState{nextState},
                                                                             mStoppedState{stoppedState},
                                                                             OnTimerExpired{onTimerExpired}
                        {
                        }

                        TimerSetState(const TimerSetState &) = delete;
                        TimerSetState &operator=(const TimerSetState &) = delete;

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
                                // Reset 'service stopped' flag
                                Stopped = false;

                                // Set the timer from a new thread with a random initial delay.
                                mFuture =
                                    std::async(
                                        std::launch::async,
                                        &TimerSetState<T>::setTimerBase,
                                        this);
                            }
                        }

                        /// @brief Inform the state that the server's service is stopped
                        void ServiceStopped()
                        {
                            Stopped = true;
                        }
                    };
                }
            }
        }
    }
}

#endif