#include "./upload.h"

namespace ara
{
    namespace diag
    {
        const uint8_t UploadService::cSid;
        const routing::TransferDirection UploadService::cTransferDirection;

        UploadService::UploadService(
            const core::InstanceSpecifier &specifier,
            ReentrancyType reentrancy,
            routing::TransferData &transferData) : routing::RequestTransfer(specifier, reentrancy, cSid, transferData, cTransferDirection)
        {
        }

        std::future<void> UploadService::RequestUpload(
            uint8_t dataFormatIdentifier,
            uint8_t addressAndLengthFormatIdentifier,
            std::vector<uint8_t> memoryAddressAndSize,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            return RequestDataTransfer(
                dataFormatIdentifier,
                addressAndLengthFormatIdentifier,
                memoryAddressAndSize,
                metaInfo,
                std::move(cancellationHandler));
        }
    }
}