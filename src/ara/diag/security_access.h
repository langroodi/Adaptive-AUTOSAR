#ifndef SECURITY_ACCESS_H
#define SECUIRTY_ACCESS_H

#include <stdint.h>
#include <future>
#include <vector>
#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "./reentrancy.h"
#include "./meta_info.h"
#include "./cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        enum class KeyCompareResultType
        {
            kKeyValid = 0x00,
            kKeyInvalid = 0x01
        };

        class SecuirtyAccess
        {
        public:
            explicit SecuirtyAccess(const ara::core::InstanceSpecifier &specifier, ReentrancyType reentrancyType);
            virtual ~SecuirtyAccess() noexcept = default;
            virtual std::future<std::vector<std::uint8_t>> GetSeed(
                std::vector<uint8_t> securityAccessDataRecord,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            virtual std::future<KeyCompareResultType> CompareKey(
                std::vector<std::uint8_t> key,
                MetaInfo &metaInfo,
                CancellationHandler cacellationHandler) = 0;
            ara::core::Result<void> Offer();
            void StopOffer();
        };
    }
}
#endif