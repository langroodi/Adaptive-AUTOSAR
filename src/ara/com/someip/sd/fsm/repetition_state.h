#ifndef REPETITION_STATE_H
#define REPETITION_STATE_H

#include <future>
#include <stdexcept>
#include <thread>
#include <cmath>
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
                    /// @brief Server's or client's service repetition state
                    /// @tparam T Server's or client state enumeration type
                    /// @note The state is not copyable
                    template <typename T>
                    class RepetitionState : public helper::MachineState<T>
                    {
                    private:
                        const T mNextState;
                        const int mRepetitionsMax;
                        const int mRepetitionsBaseDelay;
                        const std::function<void()> mOnTimerExpired;
                        uint32_t mRun;
                        std::future<void> mFuture;

                        void setTimer()
                        {
                            while (mRun < mRepetitionsMax)
                            {
                                int _doubledDelay = std::pow(2, mRun) * mRepetitionsBaseDelay;
                                auto _delay = std::chrono::milliseconds(_doubledDelay);
                                std::this_thread::sleep_for(_delay);

                                // Invoke the on timer expiration callback
                                mOnTimerExpired();
                                ++mRun;
                            }

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
                        /// @param currentState Current state at repetition phase
                        /// @param nextState Next state after repetition phase expiration
                        /// @param repetitionsMax Maximum iteration in repetition phase
                        /// @param repetitionsBaseDelay Repetition iteration delay in milliseconds
                        /// @param onTimerExpired Delegate to be invoked by timer's thread when the timer is expired
                        RepetitionState(
                            T currentState,
                            T nextState,
                            uint32_t repetitionsMax,
                            int repetitionsBaseDelay,
                            std::function<void()> onTimerExpired) noexcept : helper::MachineState<T>(currentState),
                                                                             mNextState{nextState},
                                                                             mRepetitionsMax{static_cast<int>(repetitionsMax)},
                                                                             mRepetitionsBaseDelay{repetitionsBaseDelay},
                                                                             mOnTimerExpired{onTimerExpired},
                                                                             mRun{0}
                        {
                        }

                        RepetitionState(const RepetitionState &) = delete;
                        RepetitionState &operator=(const RepetitionState &) = delete;

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
                                // Set the timer from a new thread.
                                mFuture =
                                    std::async(
                                        std::launch::async,
                                        &RepetitionState<T>::setTimer,
                                        this);
                            }
                        }
                    };
                }
            }
        }
    }
}

#endif