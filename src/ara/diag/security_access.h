#ifndef SECURITY_ACCESS_H
#define SECURITY_ACCESS_H

#include <stdint.h>
#include <future>
#include <vector>
#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "./routing/routable_uds_service.h"
#include "./reentrancy.h"
#include "./meta_info.h"
#include "./cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        /// @brief A data model that contains the status of a security level
        /// @note The structure is not ARA compatible.
        struct SecurityLevel
        {
            /// @brief Indicate whether the security level has been unlocked or not
            bool Unlocked;
            /// @brief Seed that corresponds to the security level for key comparison
            uint16_t Seed;

            explicit SecurityLevel(uint16_t seed = 0) noexcept : Unlocked{false}, Seed{seed}
            {
            }
        };

        /// @brief Security key comparison result
        enum class KeyCompareResultType
        {
            kKeyValid = 0x00,  ///< Valid security key
            kKeyInvalid = 0x01 ///< Invalid security key
        };

        /// @brief A class to evaluate the security access to the data among diagnostic services
        /// @note In contrast with the ARA standard, the class is NOT abstract.
        class SecurityAccess : routing::RoutableUdsService
        {
        private:
            static const uint8_t cSid{0x27};
            static const uint16_t cInitialSeed{1};

            const ReentrancyType mReentrancy;
            uint16_t mSeed;
            std::map<uint8_t, SecurityLevel> mSecurityLevels;

            bool tryFetchSeed(uint8_t level, uint16_t &seed) const;
            uint16_t addLevel(uint8_t level);

        public:
            /// @brief Constructor
            /// @param specifier Owner instance specifier
            /// @param reentrancyType Service reentrancy type
            explicit SecurityAccess(
                const core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType);

            ~SecurityAccess() noexcept = default;

            std::future<OperationOutput> HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override;

            /// @brief Request a seed from the client side to provide a security key
            /// @param subFunction Security level sub-function
            /// @param securityAccessDataRecord Seed request parameters payload
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
            /// @returns Provided key by the server
            std::future<std::vector<uint8_t>> GetSeed(
                uint8_t subFunction,
                std::vector<uint8_t> securityAccessDataRecord,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler);

            /// @brief Evaluate the key provided by a client
            /// @param subFunction Security level sub-function
            /// @param key Provided key
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
            /// @returns Key evaluation result from the server
            std::future<KeyCompareResultType> CompareKey(
                uint8_t subFunction,
                std::vector<uint8_t> key,
                MetaInfo &metaInfo,
                CancellationHandler &&cacellationHandler);
        };
    }
}
#endif