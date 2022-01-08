#ifndef FINITE_MACHINE_STATE_H
#define FINITE_MACHINE_STATE_H

#include <map>
#include <initializer_list>
#include "./machine_state.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            /// @brief Finite State Machine (FMS) controller
            /// @details FMS controller is responsible for transiting between added states to the machine
            /// @tparam T State enumeration type
            /// @note FSM controller is not copyable
            template <typename T>
            class FiniteStateMachine : public AbstractStateMachine<T>
            {
            private:
                std::map<T, MachineState<T> *> mStates;
                T mCurrentState;

            public:
                FiniteStateMachine() noexcept = default;
                ~FiniteStateMachine() noexcept = default;
                FiniteStateMachine(const FiniteStateMachine &) = delete;
                FiniteStateMachine &operator=(const FiniteStateMachine &) = delete;

                /// @brief Initalize the FSM
                /// @param states Machine state list
                /// @param entrypoint Entrypoint state to initialize the FSM
                void Initialize(std::initializer_list<MachineState<T> *> states, T entrypoint)
                {
                    for (auto state : states)
                    {
                        mStates.emplace(state->GetState(), state);
                        state->Register(this);
                    }

                    auto _initialState = mStates.at(entrypoint);
                    // At entrypoint the previous state and the next state are the same.
                    _initialState->Activate(entrypoint);
                    mCurrentState = entrypoint;
                }

                /// @brief Get the FSM current state
                /// @returns Current state enumeration
                T GetState() const noexcept
                {
                    return mCurrentState;
                }

                /// @brief Get the current machine state object
                /// @returns Machine state object pointer
                MachineState<T> *GetMachineState() const
                {
                    return mStates.at(mCurrentState);
                }

                void Transit(T previousState, T nextState) override
                {
                    // Only current state should be able to transit to another state
                    if (previousState == mCurrentState)
                    {
                        auto _nextMachineState = mStates.at(nextState);
                        mCurrentState = nextState;
                        _nextMachineState->Activate(previousState);
                    }
                }
            };
        }
    }
}
#endif