#ifndef READ_DATA_BY_IDENTIFIER_H
#define READ_DATA_BY_IDENTIFIER_H

#include "../../ara/diag/routing/uds_service_router.h"

namespace application
{
    namespace helper
    {
        /// @brief UDS to read data at the diagnostic server by passing the data ID (DID)
        class ReadDataByIdentifier : public ara::diag::routing::RoutableUdsService
        {
        private:
            static const uint8_t cSid{0x22};

        public:
            ReadDataByIdentifier(
                const ara::core::InstanceSpecifier &specifier) noexcept;

            std::future<ara::diag::OperationOutput> HandleMessage(
                const std::vector<uint8_t> &requestData,
                ara::diag::MetaInfo &metaInfo,
                ara::diag::CancellationHandler &&cancellationHandler) override;
        };
    }
}

#endif