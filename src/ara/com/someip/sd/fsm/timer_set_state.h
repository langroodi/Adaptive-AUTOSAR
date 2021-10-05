#ifndef TIMER_SET_STATE_H
#define TIMER_SET_STATE_H

#include <future>
#include <stdexcept>
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
                            // Transition to the next state or to the stopped state
                            if (Stopped)
                            {
                                helper::MachineState<T>::Transit(mStoppedState);
                            }
                            else
                            {
                                helper::MachineState<T>::Transit(mNextState);
                            }
                        }

                    protected:
                        /// @brief Inidicates whether the server's service stopped or not
                        bool Stopped;

                        /// @brief Delegate which is invoked by timer's thread when the timer is expired
                        const std::function<void()> OnTimerExpired;

                        /// @brief Set the phase time on state activation
                        virtual void SetTimer() = 0;

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

                        /// @brief Join to the timer's thread
                        void Join()
                        {
                            // If the future is valid, block unitl its result becomes avialable after the timer expiration.
                            if (mFuture.valid())
                            {
                                mFuture.get();
                            }
                        }

                        virtual ~TimerSetState() override
                        {
                            if (!Stopped)
                            {
                                // Set a fake stop signal, otherwise the timer loop may never end (e.g., in the main phase).
                                ServiceStopped();
                            }
                            Join();
                        }
                    };
                }
            }
        }
    }
}

#endif