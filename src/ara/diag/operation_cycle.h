#ifndef OPERATION_CYCLE_H
#define OPERATION_CYCLE_H

#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        enum class OperationCycleType
        {
            kOperationCycleStart = 0x00,
            kOperationCycleEnd = 0x01
        };

        class OperationCycle
        {
        public:
            explicit OperationCycle(const ara::core::InstanceSpecifier &specifier);
            ~OperationCycle() noexcept = default;
            ara::core::Result<OperationCycleType> GetOperationCycle();
            ara::core::Result<void> SetNotifier(
                std::function<void(OperationCycleType)> nofifier);
            ara::core::Result<void> SetOperationCycle(OperationCycleType operatuionCycle);
        };
    }
}

#endif