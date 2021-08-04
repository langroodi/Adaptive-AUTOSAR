#ifndef TRIGGER_IN_H
#define TRIGGER_IN_H

#include "./trigger.h"

namespace ara
{
    namespace sm
    {
        template <typename T>
        class TriggerIn
        {
        private:
            Trigger<T> mTrigger;

        public:
            TriggerIn() noexcept;
            ~TriggerIn();
            TriggerIn(const TriggerIn &) = delete;
            TriggerIn(TriggerIn &&) = delete;
            TriggerIn &operator=(const TriggerIn &) = delete;
            TriggerIn &operator=(TriggerIn &&) = delete;
            Trigger<T> Trigger() const noexcept;
        };
    }
}

#endif