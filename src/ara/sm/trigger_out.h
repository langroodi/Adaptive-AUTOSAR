#ifndef TRIGGER_OUT_H
#define TRIGGER_OUT_H

#include "./notifier.h"

namespace ara
{
    namespace sm
    {
        template <typename T>
        class TriggerOut
        {
        private:
            Notifier<T> mNotifier;
            
        public:
            TriggerOut();
            ~TriggerOut();
            TriggerOut(const TriggerOut &) = delete;
            TriggerOut(TriggerOut &&) = delete;
            TriggerOut &operator=(const TriggerOut &) = delete;
            TriggerOut &operator=(TriggerOut &&) = delete;
            Notifier<T> Notifier() const noexcept;
        };
    }
}

#endif