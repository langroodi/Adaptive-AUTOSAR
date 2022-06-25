#ifndef ROUTABLE_USD_SERVICE_H
#define ROUTABLE_USD_SERVICE_H

#include <functional>
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
                std::function<void(uint8_t)> mNotifier;

            protected:
                /// @brief Constructor
                /// @param sid UDS service ID
                explicit RoutableUdsService(uint8_t sid) noexcept;

            public:
                /// @brief Offer handling DM requests
                /// @returns Error result if the service has been already offered
                virtual ara::core::Result<void> Offer();

                /// @brief Set a callback to be invoked when the service has been offered
                /// @param notifier A callback to be invoked after the service offer
                void SetOfferNotifier(std::function<void(uint8_t)> notifier);

                /// @brief Stop offering request handling
                virtual void StopOffer();

                virtual ~RoutableUdsService();
            };
        }
    }
}

#endif