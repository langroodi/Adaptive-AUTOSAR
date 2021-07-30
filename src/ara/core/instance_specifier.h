#ifndef INSTANCE_SPECIFIER_H
#define INSTANCE_SPECIFIER_H

#include <string>
#include "./result.h"

namespace ara
{
    namespace core
    {
        class InstanceSpecifier final
        {
        private:
            std::string mMetaModelIdentifier;
            
        public:
            explicit InstanceSpecifier(std::string metaModelIdentifier);
            ~InstanceSpecifier() noexcept;
            static Result<InstanceSpecifier> Create(std::string metaModelIdentifier);
            bool operator==(const InstanceSpecifier &other) const noexcept;
            bool operator==(std::string other) const noexcept;
            bool operator!=(const InstanceSpecifier &other) const noexcept;
            bool operator!=(std::string other) const noexcept;
            bool operator<(const InstanceSpecifier &other) const noexcept;
            bool operator>(const InstanceSpecifier &other) const noexcept;
            std::string ToString() const noexcept;
        };
    }
}

#endif