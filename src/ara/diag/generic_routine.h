#ifndef GENERIC_ROUTINE_H
#define GENERIC_ROUTINE_H

#include "./routing/routable_uds_service.h"
#include "./reentrancy.h"

namespace ara
{
    namespace diag
    {
        /// @brief A service to execute a specific sequence of instructions (routine) and obtain the result
        /// @note The class is not fully compatible with the ARA standard.
        class GenericRoutine : public routing::RoutableUdsService
        {
        private:
            static const uint8_t cSid{0x31};

            ReentrancyType mReentrancy;

        public:
            /// @brief Constructor
            /// @param specifier Instance specifier that owns the service
            /// @param reentrancyType Service reentrancy type in a multi-threading contex
            explicit GenericRoutine(
                const core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType);

            std::future<OperationOutput> HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override;

            /// @brief Request starting a routine
            /// @param routineId Routine ID to be started
            /// @param requestData Start request byte array
            /// @param metaInfo Request handling meta-info
            /// @param cancellationHandler Request cancellation token
            virtual std::future<OperationOutput> Start(
                uint16_t routineId,
                std::vector<uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) = 0;

            /// @brief Request stopping a routine
            /// @param routineId Routine ID to be stopped
            /// @param requestData Stop request byte array
            /// @param metaInfo Request handling meta-info
            /// @param cancellationHandler Request cancellation token
            virtual std::future<OperationOutput> Stop(
                uint16_t routineId,
                std::vector<uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) = 0;

            /// @brief Request the results of a routine
            /// @param routineId Routine ID for the results request
            /// @param requestData Results request byte array
            /// @param metaInfo Request handling meta-info
            /// @param cancellationHandler Request cancellation token
            virtual std::future<OperationOutput> RequestResults(
                uint16_t routineId,
                std::vector<uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) = 0;
        };
    }
}

#endif