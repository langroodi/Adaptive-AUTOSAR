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
        /// @brief Diagnostic Trouble Code (DTC) number format
        enum class DTCFormatType : uint8_t
        {
            kDTCFormatOBD = 0,  ///< ISO 15031-6 format
            kDTCFormatUDS = 1,  ///< ISO 14229-1 format
            kDTCFormatJ1939 = 2 ///< SAE J1939-73 format
        };

        /// @brief Specific bit of the event status byte
        /// @see EventStatusByte
        /// @note Enum numeric values are not compatible with the ARA standard.
        enum class EventStatusBit : uint8_t
        {
            kTestFailed = 0,                        ///< Bit 0: Failed test
            kTestFailedThisOperationCycle = 1,      ///< Bit 1: Failed test during the current operation cycle
            kTestNotCompletedThisOperationCycle = 6 ///< Bit 6: Incomplete test during the current operation cyle
        };

        /// @brief Byte that specifies an event status
        struct EventStatusByte
        {
            /// @brief Event status byte containing the EventStatusBit
            uint8_t encodedBits;
        };

        /// @brief Debouncing state of an event
        enum class DebouncingState : uint8_t
        {
            kNeutral = 0x00,              ///< Zero Fault Detection Counter (FDC)
            kTemporarilyDefective = 0x01, ///< FDC greater than zero and less than 127
            kFinallyDefective = 0x02,     ///< FDC equal to 127
            kTemporarilyHealed = 0x04,    ///< FDC less than zero and greater than -128
            kFinallyHealed = 0x08         ///< FDC equal to -128
        };

        /// @brief Diagnostic event class that represents a fault path of a system
        /// @see Monitor
        /// @note The class interface is not fully compatible with the ARA standard.
        class Event
        {
        public:
            /// @brief Constructor
            /// @param specifier Instance specifier which owns the event
            explicit Event(const ara::core::InstanceSpecifier &specifier);

            ~Event() noexcept = default;

            /// @brief Get the event status
            /// @return Current event status
            ara::core::Result<EventStatusByte> GetEventStatus();

            /// @brief Set a notifier on the event status changed
            /// @param notifier Callback to be invoked when the event status is changed
            /// @returns Invalid argument result if the callback pointer is invalid
            ara::core::Result<void> SetEventStatusChangedNotifier(
                std::function<void(EventStatusByte)> notifier);

            /// @brief Get the Warning Indicator Requested bit status
            /// @returns Current warning indicator status
            ara::core::Result<bool> GetLatchedWIRStatus();

            /// @brief Set the Warning Indicator Requested bit status
            /// @param status Current warning indicator status
            /// @returns No error nor exception
            ara::core::Result<void> SetLatchedWIRStatus(bool status);

            /// @brief Get the Diagnostic Trouble Code (DTC) number relates to the event
            /// @param dtcFormat DTC number format
            /// @returns DTC number in the requested format
            /// @warning Currently only the UDS format is supported.
            ara::core::Result<uint32_t> GetDTCNumber(DTCFormatType dtcFormat);

            /// @brief Get the event debounding state
            /// @returns Event current debouncing state
            ara::core::Result<DebouncingState> GetDebouncingStatus();

            /// @brief Indicates whether the event has matured to test completed or not
            /// @returns True if the debouncing state is finally healed or defective, otherwise false
            ara::core::Result<bool> GetTestComplete();

            /// @brief Get the event Fault Detection Counter (FDC) value
            /// @returns Current FDC value byte
            ara::core::Result<int8_t> GetFaultDetectionCounter();
        };
    }
}

#endif