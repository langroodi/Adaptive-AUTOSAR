#ifndef DTC_INFORMATION_H
#define DTC_INFORMATION_H

#include <stdint.h>
#include <map>
#include <functional>
#include "../core/instance_specifier.h"
#include "../core/result.h"

namespace ara
{
    namespace diag
    {
        /// UDS Diagnostic Trouble Code (DTC) status byte updating state
        enum class ControlDtcStatusType : uint8_t
        {
            kDTCSettingOn = 0x00, ///< Enabled USD DTC status byte update
            kDTCSettingOff = 0x01 ///< Disable USD DTC status byte update
        };

        /// @brief Specific bit flag of the UDS DTC status byte
        /// @see UdsDtcStatusByteType
        enum class UdsDtcStatusBitType : uint8_t
        {
            kTestFailed = 0x01,                         ///< DTC test is failed at the request time
            kTestFailedThisOperationCycle = 0x02,       ///< DTC test is failed at the current operation cycle
            kPendingDTC = 0x04,                         ///< DTC test is failed at the current/preivious operation cycle
            kConfirmedDTC = 0x08,                       ///< DTC is confirmed at the request time
            kTestNotCompletedSinceLastClear = 0x10,     ///< DTC test is not completed since the last error codes clearing
            kTestFailedSinceLastClear = 0x20,           ///< DTC test is failed since the last error codes clearing
            kTestNotCompletedThisOperationCycle = 0x40, ///< DTC test is not completed at the current operation cycle
            kWarningIndicatorRequested = 0x80           ///< Warning Indicator Requested (WIR)
        };

        /// @brief Byte that specifies an UDS DTC status
        struct UdsDtcStatusByteType
        {
            /// @brief UDS DTC status byte containing the UdsDtcStatusBitType
            uint8_t encodedBits;
        };

        /// @brief A class to process Diagnostic Trouble Code (DTC) information
        /// @note Despite of the ARA standard, snapshots and overflow are not supported within this class.
        class DTCInformation
        {
        private:
            const core::InstanceSpecifier &mSpecifier;
            std::map<uint32_t, UdsDtcStatusByteType> mStatuses;
            std::function<void(uint32_t, UdsDtcStatusByteType, UdsDtcStatusByteType)> mDtcStatusChangedNotifier;
            std::function<void(uint32_t)> mNumberOfStoredEntriesNotifier;
            ControlDtcStatusType mControlDtcStatus;
            std::function<void(ControlDtcStatusType)> mControlDtcStatusNotifier;

        public:
            /// @brief Constructor
            /// @param specifier Instance specifier that owns the DTC information
            explicit DTCInformation(const core::InstanceSpecifier &specifier);

            ~DTCInformation() noexcept = default;

            /// @brief Get UDS DTC status byte of a certain DTC
            /// @param dtc DTC ID of interest
            /// @returns Requested USD DTC status byte if the DTC ID exists, otherwise an error
            core::Result<UdsDtcStatusByteType> GetCurrentStatus(uint32_t dtc);

            /// @brief Set UDS DTC status byte of a certain DTC
            /// @param dtc DTC ID of interest
            /// @param mask DTC status byte mask
            /// @param status DTC status byte
            /// @note The function is not ARA compatible.
            void SetCurrentStatus(
                uint32_t dtc, UdsDtcStatusBitType mask, UdsDtcStatusByteType status);

            /// @brief Set a notifer on any DTC status change
            /// @param notifier Callback to be invoked if the status of any DTC is changed
            /// @returns Error in case of invalid callback pointer
            core::Result<void> SetDTCStatusChangedNotifier(
                std::function<void(uint32_t, UdsDtcStatusByteType, UdsDtcStatusByteType)> notifier);

            /// @brief Get the number of stored DTC
            /// @returns Number of currently stored DTC in the primary fault memory
            core::Result<uint32_t> GetNumberOfStoredEntries();

            /// @brief Set a notifer on the number of stored DTC change
            /// @param notifier Callback to be invoked if the number of stored DTC is changed
            /// @returns Error in case of invalid callback pointer
            core::Result<void> SetNumberOfStoredEntriesNotifier(
                std::function<void(uint32_t)> notifier);

            /// @brief Clear a DTC
            /// @param dtc ID of the DTC that should be removed
            /// @returns Error if the requested DTC ID does not exist
            core::Result<void> Clear(uint32_t dtc);

            /// @brief Indicate whether the UDS DTC byte update is enabled or not
            /// @returns Control UDS status relates to the UDS service 0x85
            core::Result<ControlDtcStatusType> GetControlDTCStatus();

            /// @brief Set a notifer on the control DTC status change
            /// @param notifier Callback to be invoked if the control DTC status is changed
            /// @returns Error in case of invalid callback pointer
            core::Result<void> SetControlDtcStatusNotifier(
                std::function<void(ControlDtcStatusType)> notifier);

            /// @brief Enforce enabling the USD DTC status byte update
            /// @returns No error
            core::Result<void> EnableControlDtc();
        };
    }
}

#endif