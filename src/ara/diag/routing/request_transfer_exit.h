#ifndef REQUEST_TRANSFER_EXIT_H
#define REQUEST_TRANSFER_EXIT_H

#include "./transfer_data.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            /// @brief A class to end transfer data between diagnostic clients and the server
            class RequestTransferExit : public RoutableUdsService
            {
            private:
                static const uint8_t cSid{0x37};
                TransferData &mTransferData;

            public:
                /// @brief Constructor
                /// @param specifier Owner instance specifier
                /// @param transferData Transfer data service
                RequestTransferExit(
                    const core::InstanceSpecifier &specifier,
                    TransferData &transferData) noexcept;
                ~RequestTransferExit() noexcept = default;

                std::future<OperationOutput> HandleMessage(
                    const std::vector<uint8_t> &requestData,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler) override;
            };
        }
    }
}

#endif