#ifndef TRIGGER_H
#define TRIGGER_H

#include <functional>

namespace ara
{
    namespace sm
    {
        template <typename T>
        using TriggerHandler = std::function<void()>;

        /// @brief State changing trigger wrapper
        /// @tparam T State type
        template <typename T>
        class Trigger
        {
        private:
            T &mState;
            TriggerHandler<T> mHandler;

        public:
            /// @brief Constructor
            /// @param state State
            /// @param handler Handler to be invoked after state change
            Trigger(T &state, TriggerHandler<T> handler) : mState{state},
                                                           mHandler{handler}
            {
            }

            Trigger() = delete;
            ~Trigger() noexcept = default;
            Trigger(const Trigger &) = delete;
            Trigger(Trigger &&) = delete;
            Trigger &operator=(const Trigger &) = delete;
            Trigger &operator=(Trigger &&) = delete;

            /// @brief Write into the trigger
            /// @param state New state
            void Write(T state)
            {
                if (mState != state)
                {
                    mState = state;
                    mHandler();
                }
            }
        };
    }
}

#endif