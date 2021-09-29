#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <functional>

namespace ara
{
    namespace com
    {
        namespace helper
        {
            /// @brief Machine state abstract class
            /// @tparam T State enumeration type
            /// @note A machine state is not copyable
            template <typename T>
            class MachineState
            {
            private:
                const T mState;
                std::function<void(T, T)> mTransitionCallback;

            protected:
                /// @brief Deactivating the current state before transiting to the next state
                /// @param parameter-name description
                virtual void Deactivate(T nextState) = 0;

                /// @brief Transit to the next state
                /// @param nextState Next state coming after the current state
                void Transit(T nextState)
                {
                    Deactivate(nextState);
                    mTransitionCallback(mState, nextState);
                }

            public:
                /// @brief Constructor
                /// @param state Machine state enumeration
                MachineState(T state) noexcept : mState{state}
                {
                }

                MachineState() = delete;
                virtual ~MachineState() = default;
                MachineState(const MachineState &) = delete;
                MachineState &operator=(const MachineState &) = delete;

                /// @brief Get the machine state
                /// @returns Machine state enumeration
                T GetState() const noexcept
                {
                    return mState;
                }

                /// @brief Activate the state
                /// @param previousState Previous state before transiting to this state
                virtual Activate(T previousState) = 0;

                /// @brief Set the transition callback
                /// @param callback Delegate to be called after the current state deactivation
                void SetTransitionCallback(std::function<void(T, T)> callback);
            };
        }
    }
}

#endif