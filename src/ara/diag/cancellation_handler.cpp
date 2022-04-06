#include "./cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        CancellationHandler::CancellationHandler(bool isCanceled) noexcept : mIsCanceled{isCanceled}
        {
        }

        bool CancellationHandler::IsCanceled() const
        {
            return mIsCanceled;
        }

        void CancellationHandler::SetIsCanceled(bool isCanceled)
        {
            if (mIsCanceled != isCanceled)
            {
                mIsCanceled = isCanceled;

                if (mNotifier)
                {
                    mNotifier();
                }
            }
        }

        void CancellationHandler::SetNotifier(std::function<void()> notifier)
        {
            mNotifier = notifier;
        }
    }
}