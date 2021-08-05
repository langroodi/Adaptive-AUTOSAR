#ifndef TRIGGER_OUT_H
#define TRIGGER_OUT_H

#include "./notifier.h"

namespace ara
{
    namespace sm
    {
        /// @brief State output trigger
        /// @tparam T State type
        template <typename T>
        class TriggerOut
        {
        private:
            Notifier<T> mNotifier;

        public:
            /// @brief Constructor
            /// @param T State
            TriggerOut(T &state) : mNotifier(state)
            {
            }

            TriggerOut() = delete;
            ~TriggerOut() noexcept = default;
            TriggerOut(const TriggerOut &) = delete;
            TriggerOut(TriggerOut &&) = delete;
            TriggerOut &operator=(const TriggerOut &) = delete;
            TriggerOut &operator=(TriggerOut &&) = delete;

            /// @brief Notifier property getter
            Notifier<T> &Notifier() noexcept
            {
                return mNotifier;
            }
        };
    }
}

#endif