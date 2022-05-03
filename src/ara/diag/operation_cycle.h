#ifndef OPERATION_CYCLE_H
#define OPERATION_CYCLE_H

#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        /// @brief A class for handling a monitoring operation cycle
        class OperationCycle
        {
        private:
            const ara::core::InstanceSpecifier &mSpecifier;
            std::function<void()> mNotifier;

        public:
            /// @brief Constructor
            /// @param specifier Instance specifier that owns the operation cycle
            explicit OperationCycle(const ara::core::InstanceSpecifier &specifier);

            ~OperationCycle() noexcept = default;

            /// @brief Set a callback to be invoked when the operation cycle is restarted
            /// @param notifier Callback to be invoked after the operation cycle restart
            /// @returns No error
            /// @see RestartOperationCycle
            ara::core::Result<void> SetNotifier(std::function<void()> notifier);

            /// @brief Restart the operation cycle
            /// @returns No error
            ara::core::Result<void> RestartOperationCycle();
        };
    }
}

#endif