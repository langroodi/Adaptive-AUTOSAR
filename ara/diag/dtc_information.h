#ifndef DTC_INFORMATION_H
#define DTC_INFORMATION_H

#include <stdint.h>
#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        enum class ControlDtcStatusType : std::uint8_t
        {
            kDTCSettingOn = 0x00,
            kDTCSettingOff = 0x01
        };

        struct UdsDtcStatusByteType
        {
            /* data */
        };

        struct SnapshotRecordUpdatedType
        {
            /* data */
        };

        class DTCInformation
        {
        public:
            explicit DTCInformation(const ara::core::InstanceSpecifier &specifier);
            ~DTCInformation() noexcept = default;
            ara::core::Result<UdsDtcStatusByteType> GetCurrentStatus(std::uint32_t dtc);
            ara::core::Result<void> SetDTCStatusChangedNotifier(
                std::function<void(std::uint32_t, UdsDtcStatusByteType, UdsDtcStatusByteType)> notifier);
            ara::core::Result<void> SetSnapshotRecordUpdatedNotifier(
                std::function<void(SnapshotRecordUpdatedType)> notifier);
            ara::core::Result<std::uint32_t> GetNumberOfStoredEntries();
            ara::core::Result<void> SetNumberOfStoredEntriesNotifier(
                std::function<void(std::uint32_t)> notifier);
            ara::core::Result<void> Clear(std::uint32_t dtcGroup);
            ara::core::Result<ControlDtcStatusType> GetControlDTCStatus();
            ara::core::Result<void> SetControlDtcStatusNotifier(
                std::function<void(ControlDtcStatusType)> notifier);
            ara::core::Result<void> EnableControlDtc();
            ara::core::Result<bool> GetEventMemoryOverflow();
            ara::core::Result<void> SetEventMemoryOverflowNotifier(
                std::function<void(bool)> notifier);
        };
    }
}

#endif