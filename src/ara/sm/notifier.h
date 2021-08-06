#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <functional>
#include <vector>

namespace ara
{
    namespace sm
    {
        /// @brief Callback on notification invocation
        /// @tparam T Notified new state type
        template <typename T>
        using NotificationHandler = std::function<void(T)>;

        /// @brief State changing notifier wrapper
        /// @tparam T State type
        template <typename T>
        class Notifier
        {
        private:
            T &mState;
            std::vector<NotificationHandler<T>> mSubsribers;

        public:
            /// @brief Constructor
            /// @param state State
            Notifier(T &state) : mState{state}
            {
            }
            
            Notifier() = delete;
            ~Notifier() noexcept = default;
            Notifier(const Notifier &) = delete;
            Notifier(Notifier &&) = delete;
            Notifier &operator=(const Notifier &) = delete;
            Notifier &operator=(Notifier &&) = delete;

            /// @brief Read the current state
            /// @returns Current state
            T Read() const noexcept
            {
                return mState;
            }

            /// @brief Subscribe to the state change
            /// @param handler Handler to be invoked when the state changes
            void Subscribe(NotificationHandler<T> handler)
            {
                mSubsribers.push_back(handler);
            }

            /// @brief Notify state change
            /// @note This method should be called explicitly when the state changes
            void Notify()
            {
                for (auto subscriber : mSubsribers)
                {
                    subscriber(mState);
                }
            }
        };
    }
}
#endif