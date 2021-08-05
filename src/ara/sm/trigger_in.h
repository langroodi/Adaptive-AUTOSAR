#ifndef TRIGGER_IN_H
#define TRIGGER_IN_H

#include <utility>
#include "./trigger.h"

namespace ara
{
    namespace sm
    {
        /// @brief State input trigger
        /// @tparam T State type
        template <typename T>
        class TriggerIn
        {
        private:
            Trigger<T> mTrigger;

        public:
            /// @brief Constructor
            /// @param T State
            /// @param hanlder Handler to be invoked after state change
            TriggerIn(T &state, TriggerHandler<T> handler) : mTrigger(state, handler)
            {
            }

            TriggerIn() = delete;
            ~TriggerIn() noexcept = default;
            TriggerIn(const TriggerIn &) = delete;
            TriggerIn(TriggerIn &&) = delete;
            TriggerIn &operator=(const TriggerIn &) = delete;
            TriggerIn &operator=(TriggerIn &&) = delete;

            /// @brief Trigger property getter
            Trigger<T> &Trigger() noexcept
            {
                return mTrigger;
            }
        };
    }
}

#endif