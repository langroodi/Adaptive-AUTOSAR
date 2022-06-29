#ifndef GENERIC_UDS_SERVICE_H
#define GENERIC_UDS_SERVICE_H

#include <stdint.h>
#include "./routing/routable_uds_service.h"
#include "./reentrancy.h"

namespace ara
{
    namespace diag
    {
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
        };
    }
}

#endif