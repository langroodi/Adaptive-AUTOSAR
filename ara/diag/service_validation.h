#ifndef SERVICE_VALIDATION_H
#define SERVICE_VALIDATION_H

#include <stdint.h>
#include <future>
#include <vector>
#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "./meta_info.h"

namespace ara
{
    namespace diag
    {
        enum class ConfirmationStatusType
        {
            kResPosOk = 0x00,
            kResPosNotOk = 0x01,
            kResNegOk = 0x02,
            kResNegNotOk = 0x03,
            kResPosSuppressed = 0x04,
            kResNegSuppressed = 0x05,
            kCanceled = 0x06,
            kNoProcessingNoResponse = 0x07
        };

        class ServiceValidation
        {
        public:
            explicit ServiceValidation(const ara::core::InstanceSpecifier &specifier);
            virtual ~ServiceValidation() noexcept = default;
            virtual std::future<void> Validate(std::vector<std::uint8_t> requestData, MetaInfo &metaInfo);
            virtual std::future<void> Confirmation(ConfirmationStatusType status, MetaInfo &metaInfo);
            ara::core::Result<void> Offer();
            void StopOffer();
        };
    }
}
#endif