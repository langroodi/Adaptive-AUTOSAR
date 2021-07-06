#ifndef EXECUTION_CLIENT_H
#define EXECUTION_CLIENT_H

#include <stdint.h>
#include "../core/result.h"

namespace ara
{
    namespace exec
    {
        enum class ExecutionState : uint8_t
        {
            kRunning = 0,
            kTerminating = 1
        };

        class ExecutionClient final
        {
        public:
            ExecutionClient() noexcept;
            ~ExecutionClient() noexcept;
            ara::core::Result<void> ReportExecutionState(ExecutionState state) const noexcept;
        };
    }
}

#endif