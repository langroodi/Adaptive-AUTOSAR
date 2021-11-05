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
                        T mNextState;
                        const T mStoppedState;

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

                        /// @brief Inidicates whether the timer is interrupted or not
                        bool Interrupted;

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
                                                                    Interrupted{false}
                        {
                        }

                    public:
                        TimerSetState(const TimerSetState &) = delete;
                        TimerSetState &operator=(const TimerSetState &) = delete;

                        virtual void Activate(T previousState) override
                        {
                            // Reset 'timer interrupted' flag
                            Interrupted = false;

                            if (Stopped)
                            {
                                // Reset 'service stopped' flag
                                Stopped = false;
                                setTimerBase();
                            }
                        }

                        /// @brief Inform the state that the server's service is stopped
                        void ServiceStopped() noexcept
                        {
                            Stopped = true;
                        }

                        /// @brief Interrupt the timer
                        /// @remark If the timer is interrupted, it should transit to the next state.
                        void Interrupt() noexcept
                        {
                            Interrupted = true;
                        }

                        /// @brief Set next state
                        /// @param nextState New next state
                        void SetNextState(T nextState) noexcept
                        {
                            mNextState = nextState;
                        }

                        virtual ~TimerSetState() override
                        {
                            if (!Stopped)
                            {
                                // Set a fake stop signal, otherwise the timer loop may never end (e.g., in the main phase).
                                ServiceStopped();
                            }
                        }
                    };
                }
            }
        }
    }
}

#endif