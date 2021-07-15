#ifndef COMMUNICATION_CONTROL_H
#define COMMUNICATION_CONTROL_H

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
        struct ComCtrlRequestParamsType
        {
            /* data */
        };
        
        class CommunicationControl
        {
        public:
            explicit CommunicationControl(const ara::core::InstanceSpecifier &specifier, ReentrancyType reentrancy);
            virtual ~CommunicationControl() noexcept = default;
            virtual std::future<void> CommCtrlRequest(
                ComCtrlRequestParamsType controlType,
                MetaInfo &metaInfo,
                CancellationHandler cancellationHandler) = 0;
            ara::core::Result<void> Offer();
            void StopOffer();      
        };
    }
}
#endif