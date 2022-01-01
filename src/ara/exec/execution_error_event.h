#ifndef EXECUTION_ERROR_EVENT_H
#define EXECUTION_ERROR_EVENT_H

#include "./function_group.h"

namespace ara
{
    namespace exec
    {
        /// @brief Function group termination error code alias
        using ExecutionError = uint32_t;

        /// @brief Undefined Function Group State event argument
        struct ExecutionErrorEvent final
        {
            /// @brief Error code caused Undefined Function Group State
            ExecutionError executionError;

            /// @brief Function group pointer in the Undefined State
            const FunctionGroup *functionGroup;
        };
    }
}

#endif