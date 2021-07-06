#ifndef INSTANCE_SPECIFIER_H
#define INSTNACE_SPECIFIER_H

#include <string_view>
#include "./result.h"

namespace ara
{
    namespace core
    {
        class InstanceSpecifier final
        {
        public:
            explicit InstanceSpecifier(std::string_view metaModelIdentifier);
            ~InstanceSpecifier() noexcept;
            static Result<InstanceSpecifier> Create(std::string_view metaModelIdentifier);
            bool operator==(const InstanceSpecifier &other) const noexcept;
            bool operator==(std::string_view other) const noexcept;
            bool operator!=(const InstanceSpecifier &other) const noexcept;
            bool operator!=(std::string_view other) const noexcept;
            bool operator<(const InstanceSpecifier &other) const noexcept;
            bool operator>(const InstanceSpecifier &other) const noexcept;
            std::string_view ToString() const noexcept;
        };
    }
}

#endif