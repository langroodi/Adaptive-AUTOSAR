#ifndef TRANSFER_DATA_H
#define TRANSFER_DATA_H

#include <array>
#include "./routable_uds_service.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            /// @brief Transfer data direction between diagnostic clients and the server
            enum class TransferDirection : uint8_t
            {
                kNone = 0x00,     ///< Not specified transfer data direction
                kDownload = 0x01, ///< Transfer data from client to server
                kUpload = 0x02    ///< Transfer data from server to client
            };

            /// @brief A class to transfer data between diagnostic clients and the server
            class TransferData : public RoutableUdsService
            {
            private:
                static const uint8_t cSid{0x36};
                static const size_t cMemoryPoolSize{1024};
                static const uint8_t cInitialBlockSequenceCounter{1};

                const size_t cSequenceCounterIndex{1};
                const size_t cRequestParameterOffset{2};

                const uint8_t cWrongBlockSequenceCounter{0x73};

                TransferDirection mTransferDirection;
                size_t mMemoryAddress;
                size_t mMemorySize;
                std::array<uint8_t, cMemoryPoolSize> mMemoryPool;
                uint8_t mCurrentBlockSequenceCounter;
                uint8_t mNextBlockSequenceCounter;

                bool tryValidateSequenceCounter(
                    OperationOutput &response, uint8_t counter) const;
                bool tryTransfer(
                    uint8_t blockSequenceCounter,
                    const std::vector<std::uint8_t> &transferRequestParameterRecord,
                    std::promise<OperationOutput> &responsePromise);

            public:
                /// @brief Constructor
                /// @param specifier Owner instance specifier
                TransferData(const ara::core::InstanceSpecifier &specifier) noexcept;
                ~TransferData() noexcept = default;

                /// @brief Try to set the service transfer data configuration
                /// @param transferDirection Desired transfer data direction
                /// @param memoryAddress Memory segment address used for transfering data
                /// @param memorySize Memory segment size used for transfering data
                /// @returns True if the data transfer configuration is set successfully, otherwise false
                bool TrySetTransferConfiguration(
                    TransferDirection transferDirection,
                    size_t memoryAddress,
                    size_t memorySize) noexcept;

                /// @brief Try to reset the service transfer data configuration to default
                /// @returns False if the data transfer configuration has been already reset, otherwise true
                bool TryResetTransferConfiguration() noexcept;

                std::future<OperationOutput> HandleMessage(
                    const std::vector<uint8_t> &requestData,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler) override;
            };
        }
    }
}

#endif