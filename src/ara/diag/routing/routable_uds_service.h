#ifndef ROUTABLE_USD_SERVICE_H
#define ROUTABLE_USD_SERVICE_H

#include <future>
#include <vector>
#include "../../core/instance_specifier.h"
#include "../../core/result.h"
#include "../meta_info.h"
#include "../cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        /// @brief Positive response of a handled UDS request
        struct OperationOutput
        {
            /// @brief Response byte array
            std::vector<uint8_t> responseData;
        };

        /// @brief UDS services routing namespace
        /// @note The namespace is not part of the ARA standard
        namespace routing
        {
            /// @brief An UDS service to handle a diagnostic request by offering itself to a router
            class RoutableUdsService
            {
            private:
                const uint8_t cNegativeResponseCodeSid{0x7f};

                const uint8_t mSid;
                bool mOffered;
                const ara::core::InstanceSpecifier &mSpecifier;

            protected:
                /// @brief Constructor
                /// @param specifier Owner instance specifier
                /// @param sid UDS service ID
                RoutableUdsService(
                    const ara::core::InstanceSpecifier &specifier,
                    uint8_t sid) noexcept;

                /// @brief Generate a negative response with a specific NRC
                /// @param[out] response Generated negative response
                /// @param nrc Given Negative Response Code (NRC)
                void GenerateNegativeResponse(
                    OperationOutput &response, uint8_t nrc) const;

            public:
                /// @brief Offer handling DM requests
                /// @returns Error result if the service has been already offered
                ara::core::Result<void> Offer();

                /// @brief Get offering status of the service
                /// @returns True if the service has been offered, otherwise false
                bool IsOffered() const noexcept;

                /// @brief Handle an UDS request message
                /// @param requestData Request message byte array
                /// @param metaInfo Request message metainfo
                /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
                /// @returns Response byte array
                virtual std::future<OperationOutput> HandleMessage(
                    const std::vector<uint8_t> &requestData,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler) = 0;

                /// @brief Stop offering request handling
                void StopOffer() noexcept;

                virtual ~RoutableUdsService() noexcept = default;
            };
        }
    }
}

#endif