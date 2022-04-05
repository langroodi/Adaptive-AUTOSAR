#ifndef GENERIC_UDS_SERVICE_H
#define GENERIC_UDS_SERVICE_H

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
        /// @brief Positive response of a handled UDS request
        struct OperationOutput
        {
            /// @brief Response byte array
            std::vector<std::uint8_t> responseData;
        };

        /// @brief An abstract class to handle generic UDS messages
        class GenericUDSService
        {
        public:
            /// @brief Constructor
            /// @param specifier Owner instance specifier
            /// @param reentrancyType Service reentrancy type
            explicit GenericUDSService(
                const ara::core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType);

            virtual ~GenericUDSService() noexcept = default;

            /// @brief Offer handling DM requests
            /// @returns Error result if the service has been already offered
            ara::core::Result<void> Offer();

            /// @brief Stop offering request handling
            void StopOffer();

            /// @brief Handle an UDS request message
            /// @param sid UDS service ID
            /// @param requestData Request message byte array
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callack to be invoked when the current conversation is cancelled
            virtual std::future<OperationOutput> HandleMessage(
                std::uint8_t sid,
                std::vector<std::uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
        };
    }
}

#endif