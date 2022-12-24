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

        void InstanceSpecifier::Serialize(
            std::vector<uint8_t> &serializedObject) const
        {
            // According to AUTOSAR R22-11 SOME/IP Protocol Specification,
            // string fields should start with a field length (32-bit by default)
            // and then the serialized string.
            auto _metaModelIdLength{
                static_cast<uint32_t>(mMetaModelIdentifier.length())};

            uint8_t _byte;

            _byte = _metaModelIdLength >> 24;
            serializedObject.push_back(_byte);

            _byte = _metaModelIdLength >> 16;
            serializedObject.push_back(_byte);

            _byte = _metaModelIdLength >> 8;
            serializedObject.push_back(_byte);

            _byte = _metaModelIdLength;
            serializedObject.push_back(_byte);

            std::copy(
                mMetaModelIdentifier.cbegin(),
                mMetaModelIdentifier.cend(),
                std::back_inserter(serializedObject));
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