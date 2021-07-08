#ifndef INDICATOR_H
#define INDICATOR_H

#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        enum class IndicatorType
        {
            kOff = 0x00,
            kContinuous = 0x01,
            kBinding = 0x02,
            kBindingOrContinuous = 0x03,
            kSlowFlash = 0x04,
            kFastFlash = 0x05,
            kOnDemand = 0x06,
            kShort = 0x07
        };

        class Inidcator
        {
        public:
            explicit Inidcator(const ara::core::InstanceSpecifier &specifier);
            ~Inidcator() noexcept = default;
            ara::core::Result<IndicatorType> GetIndicator();
            ara::core::Result<void> SetNotifier(
                std::function<void(Inidcator)> notifier);
        };
    }
}

#endif