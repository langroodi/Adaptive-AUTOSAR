#ifndef CONDITION_H
#define CONDITION_H

#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        /// @brief Condition status type
        enum class ConditionType
        {
            kConditionFalse = 0x00, ///< Disabled condition
            kConditionTrue = 0x01   ///< Enabled condition
        };

        /// @brief A class that represents an enable condition for processing an event by DM
        class Condition
        {
        private:
            const ara::core::InstanceSpecifier &mSpecifier;
            ConditionType mCondition;

        public:
            /// @brief Constructor
            /// @param specifier Instance specifier that owns the condition
            explicit Condition(const ara::core::InstanceSpecifier &specifier);

            ~Condition() noexcept = default;

            /// @brief Get the current condition status
            /// @returns True result if the condition is enabled, otherwise false
            ara::core::Result<ConditionType> GetCondition();

            /// @brief Set the current condtion status
            /// @param condition Current status to be assigned to the condition
            /// @returns No error will be returned
            ara::core::Result<void> SetCondition(ConditionType condition);
        };
    }
}

#endif