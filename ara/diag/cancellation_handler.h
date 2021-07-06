#ifndef CANCELLATION_HANDLER_H
#define CANCELLATION_HANDLER_H

#include <functional>

namespace ara
{
    namespace diag
    {
        class CancellationHandler final
        {
        public:
            CancellationHandler() = delete;
            CancellationHandler(CancellationHandler &&) noexcept = default;
            CancellationHandler(CancellationHandler &) = delete;
            CancellationHandler &operator=(CancellationHandler &&) noexcept = default;
            CancellationHandler &operator=(CancellationHandler &) = delete;
            bool IsCanceled() const;
            void SetNotifier(std::function<void()> notifier);
        };
    }
}

#endif