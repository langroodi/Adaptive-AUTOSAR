#ifndef GENERIC_ROUTINE_H
#define GENERIC_ROUTINE_H

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
        struct OperationOutput
        {
            std::vector<std::uint8_t> responseData;
        };

        class GenericRoutine
        {
        public:
            explicit GenericRoutine(
                const ara::core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType);
            virtual ~GenericRoutine() noexcept = default;
            ara::core::Result<void> Offer();
            void StopOffer();
            virtual std::future<OperationOutput> Start(
                std::uint16_t routineId,
                std::vector<std::uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            virtual std::future<OperationOutput> Stop(
                std::uint16_t routineId,
                std::vector<std::uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            virtual std::future<OperationOutput> RequestResults(
                std::uint16_t routineId,
                std::vector<std::uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
        };
    }
}

#endif