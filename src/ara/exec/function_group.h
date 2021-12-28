#ifndef FUNCTION_GROUP_H
#define FUNCTION_GROUP_H

#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace exec
    {
        /// @brief A function group representative based on a manifest
        /// @note Currenly the FunctionGroup only hosts a single application
        class FunctionGroup final
        {
        private:
            core::InstanceSpecifier mInstnaceSpecifier;

            FunctionGroup(std::string metaModelIdentifier);

        public:
            /// @brief FunctionGroup factory
            /// @param metaModelIdentifier Function group manifest
            /// @returns Result containing a new instance if creation was successful, otherwise a Result containing the occurred error
            static core::Result<FunctionGroup> Create(
                std::string metaModelIdentifier);

            FunctionGroup() = delete;
            FunctionGroup(const FunctionGroup &other) = delete;
            FunctionGroup(FunctionGroup &&other);
            FunctionGroup &operator=(const FunctionGroup &other) = delete;
            FunctionGroup &operator=(FunctionGroup &&other);
            ~FunctionGroup() noexcept = default;

            /// @brief Get the instance specifier
            /// @returns Application shortname-path that belongs to the function group
            const core::InstanceSpecifier& GetInstance() const noexcept;

            inline bool operator==(const FunctionGroup &other) const noexcept
            {
                return mInstnaceSpecifier == other.mInstnaceSpecifier;
            }

            inline bool operator!=(const FunctionGroup &other) const noexcept
            {
                return mInstnaceSpecifier != other.mInstnaceSpecifier;
            }
        };
    }
}

#endif