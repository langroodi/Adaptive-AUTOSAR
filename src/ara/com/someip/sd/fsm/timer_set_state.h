#ifndef TIMER_SET_STATE_H
#define TIMER_SET_STATE_H

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
                /// @brief Finite state machine
                /// @note The namespace is not part of the official AUTOSAR standard.
                namespace fsm
                {
                    /// @brief Server's or client's service timer set state
                    /// @tparam T Server's or client state enumeration type
                    /// @note The state is not copyable
                    template <typename T>
                    class TimerSetState : virtual public helper::MachineState<T>
                    {
                    private:
                        const T mStoppedState;
                        T mNextState;
                        bool mStopped;
                        bool mInterrupted;

                        void setTimerBase()
                        {
                            SetTimer();
                            // Transition to the next state or to the stopped state
                            if (mStopped)
                            {
                                helper::MachineState<T>::Transit(mStoppedState);
                            }
                            else
                            {
                                helper::MachineState<T>::Transit(mNextState);
                            }
                        }

                    protected:
                        /// @brief Wait for certian period of time
                        /// @param duration Waiting duration
                        /// @returns True if waiting is interrupted; otherwise false if timeout occurs
                        bool WaitFor(std::chrono::milliseconds duration)
                        {
                            std::this_thread::sleep_for(duration);
                            bool _result = mStopped || mInterrupted;

                            return _result;
                        }

                        /// @brief Wait for certian period of time
                        /// @param duration Waiting duration in milliseconds
                        /// @returns True if waiting is interrupted; otherwise false if timeout occurs
                        bool WaitFor(int duration)
                        {
                            auto _milliseconds = std::chrono::milliseconds(duration);
                            bool _result = WaitFor(_milliseconds);

                            return _result;
                        }

                        /// @brief Interrupt the timer
                        /// @remark If the timer is interrupted, it should transit to the next state.
                        void Interrupt() noexcept
                        {
                            mInterrupted = true;
                        }

                        /// @brief Delegate which is invoked by timer's thread when the timer is expired
                        const std::function<void()> OnTimerExpired;

                        /// @brief Set the phase time on state activation
                        virtual void SetTimer() = 0;

                        /// @brief Constructor
                        /// @param nextState Next state after initial wait phase expiration
                        /// @param stoppedState Default stopped state after put a stop to the service
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        TimerSetState(
                            T nextState,
                            T stoppedState,
                            std::function<void()> onTimerExpired) : mNextState{nextState},
                                                                    mStoppedState{stoppedState},
                                                                    OnTimerExpired{onTimerExpired},
                                                                    mStopped{false},
                                                                    mInterrupted{false}
                        {
                        }

                        void Deactivate(T nextState) override
                        {
                            // Reset 'service interrupted' flag
                            mInterrupted = false;
                            // Reset 'service stopped' flag
                            mStopped = false;
                        }

                    public:
                        TimerSetState(const TimerSetState &) = delete;
                        TimerSetState &operator=(const TimerSetState &) = delete;

                        virtual void Activate(T previousState) override
                        {
                            if (!mStopped)
                            {
                                setTimerBase();
                            }
                        }

                        /// @brief Inform the state that the server's service is stopped
                        void ServiceStopped() noexcept
                        {
                            mStopped = true;
                        }

                        /// @brief Set next state
                        /// @param nextState New next state
                        void SetNextState(T nextState) noexcept
                        {
                            mNextState = nextState;
                        }

                        virtual ~TimerSetState() override
                        {
                            // Set a fake stop signal, otherwise the timer loop may never end (e.g., in the main phase).
                            ServiceStopped();
                        }
                    };
                }
            }
        }
    }
}

#endif