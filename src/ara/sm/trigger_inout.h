#ifndef TRIGGER_INOUT_H
#define TRIGGER_INOUT_H

#include "./trigger.h"
#include "./notifier.h"

namespace ara
{
    namespace sm
    {
        template <typename T>
        class TriggerInOut
        {
        private:
            Trigger<T> mTrigger;
            Notifier<T> mNotifier;

        public:
            TriggerInOut();
            ~TriggerInOut();
            TriggerInOut(const TriggerInOut &) = delete;
            TriggerInOut(TriggerInOut &&) = delete;
            TriggerInOut &operator=(const TriggerInOut &) = delete;
            TriggerInOut &operator=(TriggerInOut &&) = delete;
            Trigger<T> Trigger() const noexcept;
            Notifier<T> Notifier() const noexcept;
        };
    }
}

#endif