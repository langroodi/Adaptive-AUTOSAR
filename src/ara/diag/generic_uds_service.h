#ifndef GENERIC_UDS_SERVICE_H
#define GENERIC_UDS_SERVICE_H

#include <stdint.h>
#include <vector>
#include <future>
#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "./reentrancy.h"
#include "./meta_info.h"
#include "./cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        struct OperationOutput
        {
            std::vector<std::uint8_t> responseData;
        };

        class GenericUDSService
        {
        public:
            explicit GenericUDSService(
                const ara::core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType);
            virtual ~GenericUDSService() noexcept = default;
            ara::core::Result<void> Offer();
            void StopOffer();
            virtual std::future<OperationOutput> HandleMessage(
                std::uint8_t sid,
                std::vector<std::uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
        };
    }
}

#endif