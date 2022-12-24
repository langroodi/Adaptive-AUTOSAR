#ifndef INSTANCE_SPECIFIER_H
#define INSTANCE_SPECIFIER_H

#include <string>
#include <vector>
#include "./result.h"

namespace ara
{
    namespace core
    {
        /// @brief AUTOSAR shortname-path wrapper
        class InstanceSpecifier final
        {
        private:
            std::string mMetaModelIdentifier;

        public:
            /// @brief Constructor
            /// @param metaModelIdentifier Shortname-path
            explicit InstanceSpecifier(std::string metaModelIdentifier);

            InstanceSpecifier(const InstanceSpecifier &other);
            InstanceSpecifier(InstanceSpecifier &&other) noexcept;

            InstanceSpecifier &operator=(const InstanceSpecifier &other);
            InstanceSpecifier &operator=(InstanceSpecifier &&other);

            InstanceSpecifier() = delete;
            ~InstanceSpecifier() noexcept = default;

            /// @brief InstanceSpecifier factory
            /// @param metaModelIdentifier Shortname-path
            /// @returns Result containing the created InstanceSpecifier
            static Result<InstanceSpecifier> Create(std::string metaModelIdentifier);

            inline bool operator==(const InstanceSpecifier &other) const noexcept
            {
                return mMetaModelIdentifier == other.mMetaModelIdentifier;
            }

            inline bool operator==(std::string other) const noexcept
            {
                return mMetaModelIdentifier == other;
            }

            inline bool operator!=(const InstanceSpecifier &other) const noexcept
            {
                return mMetaModelIdentifier != other.mMetaModelIdentifier;
            }

            inline bool operator!=(std::string other) const noexcept
            {
                return mMetaModelIdentifier != other;
            }

            inline bool operator<(const InstanceSpecifier &other) const noexcept
            {
                return mMetaModelIdentifier < other.mMetaModelIdentifier;
            }

            inline bool operator>(const InstanceSpecifier &other) const noexcept
            {
                return mMetaModelIdentifier > other.mMetaModelIdentifier;
            }

            /// @brief Convert the instance to a string
            /// @returns Meta-model ID (Shortname-path)
            std::string ToString() const noexcept;

            /// @brief Serialized the object
            /// @param[out] serializedObject Serialized object byte vector
            /// @note This is not an ARA specified method.
            void Serialize(std::vector<uint8_t> &serializedObject) const;
        };

        inline bool operator==(std::string lhs, const InstanceSpecifier &rhs) noexcept
        {
            return lhs == rhs.ToString();
        }

        inline bool operator!=(std::string lhs, const InstanceSpecifier &rhs) noexcept
        {
            return lhs != rhs.ToString();
        }
    }
}

#endif