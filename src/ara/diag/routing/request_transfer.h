#ifndef REQUEST_TRANSFER_H
#define REQUEST_TRANSFER_H

#include "../reentrancy.h"
#include "./transfer_data.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            /// @brief Request data transfer in a certian direction (download/upload)
            class RequestTransfer : public RoutableUdsService
            {
            private:
                const size_t cNibbleBitLength{4};
                const uint8_t cRequestOutOfRange{0x31};
                const uint8_t cUploadDownloadNotAccepted{0x70};
                const std::string cMaxNumberOfBlockLengthKey{"MaxNumberOfBlockLength"};

                const ReentrancyType mReentrancy;
                TransferData &mTransferData;
                const TransferDirection mTransferDirection;

            protected:
                /// @brief Constructor
                /// @param specifier Owner instance specifier
                /// @param reentrancyType Service reentrancy type
                /// @param sid UDS service ID
                /// @param transferData Reference to a TransferData service
                /// @param transferDirection Data transfer direction
                /// @throws std::invalid_argument Throws when the transfer direction is not determined
                RequestTransfer(
                    const ara::core::InstanceSpecifier &specifier,
                    ReentrancyType reentrancyType,
                    uint8_t sid,
                    TransferData &transferData,
                    TransferDirection transferDirection);

                /// @brief Try to parse a transfer request
                /// @param[in] requestData Transfer request byte array to be parsed
                /// @param[out] dataFormatIdentifier Parsed data transfer format ID
                /// @param[out] addressAndLengthFormatIdentifier Parsed memory address and size format ID
                /// @param[out] memoryAddressAndSize Parsed memory and size length byte array
                /// @returns True if the request is parsed successfully, otherwise false
                bool TryParseRequest(
                    const std::vector<uint8_t> &requestData,
                    uint8_t &dataFormatIdentifier,
                    uint8_t &addressAndLengthFormatIdentifier,
                    std::vector<uint8_t> &memoryAddressAndSize) const;

                /// @brief Try to parse a memory length format
                /// @param[in] dataFormatIdentifier Data transfer encryption and/or compression format ID
                /// @param[in] addressAndLengthFormatIdentifier Parsed memory address and size format ID
                /// @param[in] memoryAddressAndSize Memory address and size length byte array to be parsed
                /// @param[out] memoryAddress Parsed memory address for data transfer
                /// @param[out] memorySize Parsed memory size for data transfer
                /// @returns True if the length format is parsed successfully, otherwise false
                bool TryParseLengthFormat(
                    uint8_t dataFormatIdentifier,
                    uint8_t addressAndLengthFormatIdentifier,
                    const std::vector<uint8_t> &memoryAddressAndSize,
                    size_t &memoryAddress,
                    size_t &memorySize) const;

                /// @brief Try to request a transfer in a certain direction (download/upload)
                /// @param memoryAddress Memory address for transfer
                /// @param memorySize Memory size for transfer
                /// @returns True if the request accepted, otherwise false
                bool TryRequestTransfer(size_t memoryAddress, size_t memorySize);

                /// @brief Try to generate a positive response to a transfer request
                /// @param[in] metaInfo Message handling meta-info
                /// @param[out] response Generated positive response
                /// @returns True if the positive response is generated successfully, otherwise false
                bool TryGeneratePositiveResponse(
                    MetaInfo &metaInfo, OperationOutput &response) const;

                /// @brief Request diagnostic data transfer
                /// @param dataFormatIdentifier Diagnostic data compression and/or encryption format ID
                /// @param addressAndLengthFormatIdentifier Memory address and size length format ID
                /// @param memoryAddressAndSize Memory address and size length byte array
                /// @param metaInfo Message handling meta-info
                /// @param cancellationHandler Message handling cancellation token
                /// @returns Future with a NRC exception in case of error
                std::future<void> RequestDataTransfer(
                    uint8_t dataFormatIdentifier,
                    uint8_t addressAndLengthFormatIdentifier,
                    std::vector<uint8_t> memoryAddressAndSize,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler);

            public:
                std::future<OperationOutput> HandleMessage(
                    const std::vector<uint8_t> &requestData,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler) override;

                RequestTransfer() = delete;
            };
        }
    }
}

#endif