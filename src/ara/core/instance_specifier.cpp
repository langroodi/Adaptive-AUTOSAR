#include "./instance_specifier.h"

namespace ara
{
    namespace core
    {
        InstanceSpecifier::InstanceSpecifier(
            std::string metaModelIdentifier) : mMetaModelIdentifier{metaModelIdentifier}
        {
        }

        InstanceSpecifier::~InstanceSpecifier() noexcept
        {
        }

        bool InstanceSpecifier::operator==(const InstanceSpecifier &other) const noexcept
        {
            return mMetaModelIdentifier == other.mMetaModelIdentifier;
        }

        bool InstanceSpecifier::operator==(std::string other) const noexcept
        {
            return mMetaModelIdentifier == other;
        }

        bool InstanceSpecifier::operator!=(const InstanceSpecifier &other) const noexcept
        {
            return mMetaModelIdentifier != other.mMetaModelIdentifier;
        }
        bool InstanceSpecifier::operator!=(std::string other) const noexcept
        {
            return mMetaModelIdentifier != other;
        }

        std::string InstanceSpecifier::ToString() const noexcept
        {
            return mMetaModelIdentifier;
        }
    }
}