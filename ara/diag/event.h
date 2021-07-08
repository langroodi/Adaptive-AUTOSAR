#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        enum class DTCFormatType : std::uint8_t
        {
            kDTCFormatOBD = 0,
            kDTCFormatUDS = 1,
            kDTCFormatJ1939 = 2
        };
        
        enum class DebouncingState : std::uint8_t
        {
            kNeutral = 0x00,
            kTemporarilyDefective = 0x01,
            kFinallyDefective = 0x02,
            kTemporarilyHealed = 0x04,
            kFinallyHealed = 0x08
        };

        class Event
        {
        public:
            using EventStatusByte = std::uint8_t;

            explicit Event(const ara::core::InstanceSpecifier &specifier);
            ~Event() noexcept = default;
            ara::core::Result<EventStatusByte> GetEventStatus();
            ara::core::Result<void> SetEventStatusChangedNotifier(std::function<void(EventStatusByte)> notifier);
            ara::core::Result<bool> GetLatchedWIRStatus ();
            ara::core::Result<void> SetLatchedWIRStatus (bool status);
            ara::core::Result<std::uint32_t> GetDTCNumber (DTCFormatType dtcFormat);
            ara::core::Result<DebouncingState> GetDebouncingStatus ();
            ara::core::Result<bool> GetTestComplete ();
            ara::core::Result<std::int8_t> GetFaultDetectionCounter ();
        };
    }
}

#endif