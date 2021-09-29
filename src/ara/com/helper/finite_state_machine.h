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
            class FiniteStateMachine
            {
            private:
                std::map<T, MachineState> mStates;

                void transit(T previousState, T nextState)
                {
                    MachineState _nextMachineState = mStates.at(nextState);
                    _nextMachineState.Activate(previousState);
                }

            public:
                /// @brief Constructor
                /// @param states Machine state list
                /// @param entrypoint Entrypoint state to initialize the FSM
                FiniteStateMachine(
                    std::initializer_list<MachineState &&> states, T entrypoint)
                {
                    auto _callback =
                        std::bind(
                            &transit,
                            this,
                            std::placeholders::_1,
                            std::placeholders::_2);

                    for (auto state : states)
                    {
                        mStates.insert(state.GetState(), state);
                        state.SetTransitionCallback(_callback);
                    }

                    MachineState _initialState = mStates.at(entrypoint);
                    // At entrypoint the previous state and the next state are the same.
                    _initialState.Activate(entrypoint);
                }

                FiniteStateMachine() = delete;
                ~FiniteStateMachine() = default;
                FiniteStateMachine(const FiniteStateMachine &) = delete;
                FiniteStateMachine &operator=(const FiniteStateMachine &) = delete;
            };
        }
    }
}
#endif