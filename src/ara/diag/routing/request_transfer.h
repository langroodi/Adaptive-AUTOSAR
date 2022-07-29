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
                const uint8_t cDataFormatIdentifier{0x00};
                const uint8_t cMemoryAddressLength{2};
                const uint8_t cMemorySizeLength{2};

                const std::string cMaxNumberOfBlockLengthKey{"MaxNumberOfBlockLength"};

                const ReentrancyType mReentrancy;
                TransferData &mTransferData;

            protected:
                /// @brief Constructor
                /// @param specifier Owner instance specifier
                /// @param reentrancyType Service reentrancy type
                /// @param transferData Reference to a TransferData service
                /// @param sid UDS service ID
                RequestTransfer(
                    const ara::core::InstanceSpecifier &specifier,
                    ReentrancyType reentrancyType,
                    TransferData &transferData,
                    uint8_t sid);

                /// @brief Try to request a transfer in a certain direction (download/upload)
                /// @param dataFormatIdentifier Data encryption and/or compression format ID
                /// @param addressAndLengthFormatIdentifier Memory address and size length format ID
                /// @param memoryAddress Memory address for transfer
                /// @param memorySize Memory size for transfer
                /// @returns True if the request accepted, otherwise false
                bool TryRequestTransfer(
                    uint8_t dataFormatIdentifier,
                    uint8_t addressAndLengthFormatIdentifier,
                    size_t memoryAddress,
                    size_t memorySize);
            };
        }
    }
}

#endif