#ifndef ROUTABLE_USD_SERVICE_H
#define ROUTABLE_USD_SERVICE_H

#include <functional>
#include "../../core/instance_specifier.h"
#include "../../core/result.h"

namespace ara
{
    namespace diag
    {
        /// @brief UDS services routing namespace
        /// @note The namespace is not part of the ARA standard
        namespace routing
        {
            /// @brief An UDS service to handle a diagnostic request by offering itself to a router
            class RoutableUdsService
            {
            private:
                const uint8_t mSid;
                bool mOffered;
                std::function<void(uint8_t, bool)> mNotifier;

            protected:
                /// @brief Instance specifier that owns the service
                const ara::core::InstanceSpecifier &Specifier;

                /// @brief Constructor
                /// @param specifier Owner instance specifier
                /// @param sid UDS service ID
                RoutableUdsService(
                    const ara::core::InstanceSpecifier &specifier,
                    uint8_t sid) noexcept;

            public:
                /// @brief Offer handling DM requests
                /// @returns Error result if the service has been already offered
                ara::core::Result<void> Offer();

                /// @brief Set a callback to be invoked when the service offering state changed
                /// @param notifier A callback to be invoked after the service being offered or stop being offered
                void SetOfferNotifier(std::function<void(uint8_t, bool)> notifier);

                /// @brief Stop offering request handling
                void StopOffer();

                virtual ~RoutableUdsService();
            };
        }
    }
}

#endif