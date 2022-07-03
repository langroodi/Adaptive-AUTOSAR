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

            const ReentrancyType mReentrancy;

        public:
            /// @brief Constructor
            /// @param specifier Owner instance specifier
            /// @param reentrancyType Service reentrancy type
            explicit SecurityAccess(
                const ara::core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType);

            virtual ~SecurityAccess() noexcept = default;

            /// @brief Request a seed from the client side to provide a security key
            /// @param securityAccessDataRecord Seed request parameters payload
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
            /// @returns Provided key by the server
            virtual std::future<std::vector<uint8_t>> GetSeed(
                std::vector<uint8_t> securityAccessDataRecord,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler);

            /// @brief Evaluate the key provided by a client
            /// @param key Provided key
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
            /// @returns Key evaluation result from the server
            virtual std::future<KeyCompareResultType> CompareKey(
                std::vector<uint8_t> key,
                MetaInfo &metaInfo,
                CancellationHandler cacellationHandler);
        };
    }
}
#endif