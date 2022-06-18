#ifndef META_INFO_H
#define META_INFO_H

#include <stdint.h>
#include <map>
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
            kDiagnosticCommunication, ///< For service request in classic platform Diagnostic Communication Manager (DCM)
            kFaultMemory,             ///< For Data IDentifier (DID) according to ISO 14229-1 in diagnostic snapshots
            kDoIP                     ///< For reading Vehicle Identifier Number (VIN) according to ISO 3779
        };

        /// @brief Diagnostic metainfo in different context
        class MetaInfo final
        {
        private:
            Context mContext;
            std::map<std::string, std::string> mValues;

        public:
            MetaInfo() noexcept = delete;
            MetaInfo(const MetaInfo &) = delete;
            MetaInfo(MetaInfo &&) noexcept = default;

            /// @brief Non-ARA compatible constructor
            /// @param context Metainfo context
            MetaInfo(Context context);

            MetaInfo &operator=(const MetaInfo &) = delete;
            MetaInfo &operator=(MetaInfo &&) & = default;

            /// @brief Get an entry value within the metainfo
            /// @param key Entry key
            /// @returns Optional entry value based on the key existence
            core::Optional<std::string> GetValue(std::string key);

            /// @brief Add a key-value pair to the metainfo
            /// @param key Key of the pair
            /// @param value Value of the pair
            /// @note The method is not defined in the ARA standard.
            void SetValue(std::string key, std::string value);

            /// @brief Get metainfo context
            /// @returns Context in which the metainfo has been defined
            Context GetContext() const noexcept;

            ~MetaInfo() = default;
        };
    }
}

#endif