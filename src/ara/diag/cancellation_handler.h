#ifndef CANCELLATION_HANDLER_H
#define CANCELLATION_HANDLER_H

#include <functional>

namespace ara
{
    namespace diag
    {
        /// @brief A wrapper class around a conversation cancellation handler callback
        class CancellationHandler final
        {
        private:
            bool mIsCanceled;
            std::function<void()> mNotifier;

        public:
            CancellationHandler() = delete;
            CancellationHandler(CancellationHandler &&) noexcept = default;
            CancellationHandler(CancellationHandler &) = delete;
            CancellationHandler &operator=(CancellationHandler &&) noexcept = default;
            CancellationHandler &operator=(CancellationHandler &) = delete;

            /// @brief Constructor
            /// @param isCanceled Initial cancellation status
            /// @note The constructor is not ARA compatible.
            CancellationHandler(bool isCanceled) noexcept;

            /// @brief Get conversation cancellation status
            /// @returns True if DM canceled the conversation, otherwise false
            bool IsCanceled() const;

            /// @brief Set conversation cancellation status
            /// @param isCanceled Current conversation cancellation status
            /// @note The method is not ARA compatible.
            void SetIsCanceled(bool isCanceled);

            /// @brief Set a callback to be invoked when DM canceled the conversation
            /// @param notifier A callback to be notified after the cancellation
            void SetNotifier(std::function<void()> notifier);
        };
    }
}

#endif