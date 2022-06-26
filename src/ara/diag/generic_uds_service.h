#ifndef GENERIC_UDS_SERVICE_H
#define GENERIC_UDS_SERVICE_H

#include <stdint.h>
#include <vector>
#include <future>
#include "./routing/routable_uds_service.h"
#include "./reentrancy.h"
#include "./meta_info.h"
#include "./cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        /// @brief Positive response of a handled UDS request
        struct OperationOutput
        {
            /// @brief Response byte array
            std::vector<std::uint8_t> responseData;
        };

        /// @brief An abstract class to handle generic UDS messages
        /// @note The class signature is not aligned with the ARA standard.
        class GenericUDSService : public routing::RoutableUdsService
        {
        private:
            const ReentrancyType mReentrancy;

        public:
            /// @brief Constructor
            /// @param specifier Owner instance specifier
            /// @param reentrancyType Service reentrancy type
            /// @param sid UDS service ID
            explicit GenericUDSService(
                const ara::core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType,
                std::uint8_t sid) : routing::RoutableUdsService(specifier, sid),
                                    mReentrancy{reentrancyType}
            {
            }

            /// @brief Handle an UDS request message
            /// @param requestData Request message byte array
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callack to be invoked when the current conversation is cancelled
            virtual std::future<OperationOutput> HandleMessage(
                std::vector<std::uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
        };
    }
}

#endif