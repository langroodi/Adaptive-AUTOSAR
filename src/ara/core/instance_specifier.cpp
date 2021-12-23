#include "./instance_specifier.h"

namespace ara
{
    namespace core
    {
        InstanceSpecifier::InstanceSpecifier(
            std::string metaModelIdentifier) : mMetaModelIdentifier{metaModelIdentifier}
        {
        }

        InstanceSpecifier::InstanceSpecifier(
            const InstanceSpecifier &other) : mMetaModelIdentifier{other.mMetaModelIdentifier}
        {
        }

        InstanceSpecifier::InstanceSpecifier(
            InstanceSpecifier &&other) noexcept : mMetaModelIdentifier{std::move(other.mMetaModelIdentifier)}
        {
        }

        InstanceSpecifier &InstanceSpecifier::operator=(
            const InstanceSpecifier &other)
        {
            mMetaModelIdentifier = other.mMetaModelIdentifier;
            return *this;
        }
        InstanceSpecifier &InstanceSpecifier::operator=(
            InstanceSpecifier &&other)
        {
            mMetaModelIdentifier = std::move(other.mMetaModelIdentifier);
            return *this;
        }

        std::string InstanceSpecifier::ToString() const noexcept
        {
            return mMetaModelIdentifier;
        }

        Result<InstanceSpecifier> InstanceSpecifier::Create(
            std::string metaModelIdentifier)
        {
            InstanceSpecifier _instanceSpecifier{metaModelIdentifier};
            Result<InstanceSpecifier> _result{std::move(_instanceSpecifier)};

            return _result;
        }
    }
}