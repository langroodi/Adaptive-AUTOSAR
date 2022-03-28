#ifndef META_INFO_H
#define META_INFO_H

#include <stdint.h>
#include <string>
#include "../core/optional.h"

namespace ara
{
    /// @brief ARA diagnostic management
    namespace diag
    {
        /// @brief Diagnostic metainfo context
        enum class Context : uint32_t
        {
            kDiagnosticCommunication,   ///< For service request in classic platform Diagnostic Communication Manager (DCM)
            kFaultMemory,               ///< For Data IDentifier (DID) according to ISO 14229-1 in diagnostic snapshots
            kDoIP                       ///< For reading Vehicle Identifier Number (VIN) according to ISO 3779
        };

        /// @brief Diagnostic metainfo in different context
        class MetaInfo final
        {
        public:
            MetaInfo() noexcept = delete;
            MetaInfo(const MetaInfo &) = delete;
            MetaInfo(MetaInfo &&) noexcept;

            MetaInfo &operator=(const MetaInfo &) = delete;
            MetaInfo &operator=(MetaInfo &&) &noexcept;

            /// @brief Get an entry value within the metainfo
            /// @param key Entry key
            /// @returns Optional entry value based on the key existence
            core::Optional<std::string> GetValue(std::string key) const noexcept;

            /// @brief Get metainfo context
            /// @returns Context in which the metainfo has been defined
            Context GetContext() const noexcept;

            ~MetaInfo();
        };
    }
}

#endif