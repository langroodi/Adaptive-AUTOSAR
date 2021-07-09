#ifndef ECU_RESET_REQUEST_H
#define ECU_RESET_REQUEST_H

#include <stdint.h>
#include <future>
#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "../core/optional.h"
#include "./meta_info.h"
#include "./cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        enum class LastResetType : std::uint32_t
        {
            kRegular = 0,
            kUnexpected = 1,
            kSoftReset = 2,
            kHardReset = 3,
            kKeyOffOnReset = 4,
            kCustomReset = 5
        };

        enum class ResetRequestType : std::uint32_t
        {
            kSoftReset = 1,
            kHardReset = 2,
            kKeyOffOnReset = 3,
            kCustomReset = 4
        };

        class EcuResetRequest
        {
        public:
            explicit EcuResetRequest(const ara::core::InstanceSpecifier &specifier);
            virtual ~EcuResetRequest() noexcept = default;
            ara::core::Result<void> Offer();
            void StopOffer();
            virtual ara::core::Result<LastResetType> GetLastResetCause() = 0;
            virtual std::future<void> RequestReset(
                ResetRequestType resetType,
                ara::core::Optional<std::uint8_t> id,
                const MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            virtual void ExecuteReset(MetaInfo metaInfo) = 0;
            virtual std::future<void> EnableRapidShutdown(
                bool enable,
                const MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
        };
    }
}
#endif