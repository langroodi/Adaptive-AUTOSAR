#ifndef UPLOAD_H
#define UPLOAD_H

#include "./routing/request_transfer.h"

namespace ara
{
    namespace diag
    {
        /// @brief A class to request diagnostic data transfer from the server to a client
        /// @note The class is not compatible with the ARA standard.
        class UploadService : public routing::RequestTransfer
        {
        private:
            static const uint8_t cSid{0x35};
            static const routing::TransferDirection cTransferDirection{
                routing::TransferDirection::kUpload};

        public:
            /// @brief Constructor
            /// @param specifier Instance specifier that owns the service
            /// @param reentrancy Service reentrancy type
            /// @param transferData Reference to the transfer data service
            explicit UploadService(
                const core::InstanceSpecifier &specifier,
                ReentrancyType reentrancy,
                routing::TransferData &transferData);

            /// @brief Request diagnostic data upload
            /// @param dataFormatIdentifier Diagnostic data compression and/or encryption format ID
            /// @param addressAndLengthFormatIdentifier Memory address and size length format ID
            /// @param memoryAddressAndSize Memory address and size length byte array
            /// @param metaInfo Message handling meta-info
            /// @param cancellationHandler Message handling cancellation token
            /// @returns Future with a NRC exception in case of error
            std::future<void> RequestUpload(
                uint8_t dataFormatIdentifier,
                uint8_t addressAndLengthFormatIdentifier,
                std::vector<uint8_t> memoryAddressAndSize,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler);
        };
    }
}

#endif