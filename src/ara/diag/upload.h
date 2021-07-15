#ifndef UPLOAD_H
#define UPLOAD_H

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
            /* data */
        };

        class UploadService
        {
        public:
            std::vector<std::uint8_t> responseData;

            explicit UploadService(const ara::core::InstanceSpecifier &specifier, ReentrancyType reentrancy);
            virtual ~UploadService() noexcept = default;
            virtual std::future<void> RequestUpload(
                std::uint8_t dataFormatIdentifier,
                std::uint8_t addressAndLengthFormatIdentifier,
                std::vector<std::uint8_t> memoryAddressAndSize,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            virtual std::vector<OperationOutput> UploadData(
                std::size_t numBytesToReturn,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            virtual std::future<OperationOutput> RequestUploadExit(
                std::vector<std::uint8_t> transferRequestParameterRecord,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            ara::core::Result<void> Offer();
            void StopOffer();      
        };
    }
}

#endif