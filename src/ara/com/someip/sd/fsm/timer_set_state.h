#ifndef TIMER_SET_STATE_H
#define TIMER_SET_STATE_H

#include <mutex>
#include <condition_variable>
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
                        std::mutex mMutex;
                        std::unique_lock<std::mutex> mLock;
                        std::condition_variable mConditionVariable;
                        bool mStopped;

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
                        /// @param durtation Waiting duration
                        /// @returns True if waiting is interrupted; otherwise false if timeout occurs
                        bool WaitFor(std::chrono::milliseconds duration)
                        {
                            mLock.lock();
                            std::cv_status _status =
                                mConditionVariable.wait_for(
                                    mLock, duration);
                            mLock.unlock();
                            bool _result = _status != std::cv_status::timeout;

                            return _result;
                        }

                        /// @brief Wait for certian period of time
                        /// @param durtation Waiting duration in milliseconds
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
                            mConditionVariable.notify_one();
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
                                                                    mLock(mMutex, std::defer_lock),
                                                                    mStopped{true}
                        {
                        }

                    public:
                        TimerSetState(const TimerSetState &) = delete;
                        TimerSetState &operator=(const TimerSetState &) = delete;

                        virtual void Activate(T previousState) override
                        {
                            if (mStopped)
                            {
                                // Reset 'service stopped' flag
                                mStopped = false;
                                setTimerBase();
                            }
                        }

                        /// @brief Inform the state that the server's service is stopped
                        void ServiceStopped() noexcept
                        {
                            mStopped = true;
                            mConditionVariable.notify_one();
                        }

                        /// @brief Set next state
                        /// @param nextState New next state
                        void SetNextState(T nextState) noexcept
                        {
                            mNextState = nextState;
                        }

                        virtual ~TimerSetState() override
                        {
                            if (!mStopped)
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