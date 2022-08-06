#include "./download.h"

namespace ara
{
    namespace diag
    {
        const uint8_t DownloadService::cSid;
        const routing::TransferDirection DownloadService::cTransferDirection;

        DownloadService::DownloadService(
            const core::InstanceSpecifier &specifier,
            ReentrancyType reentrancy,
            routing::TransferData &transferData) : routing::RequestTransfer(specifier, reentrancy, cSid, transferData, cTransferDirection)
        {
        }

        std::future<void> DownloadService::RequestDownload(
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