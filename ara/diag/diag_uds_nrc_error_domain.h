#ifndef DIAG_UDS_NRC_ERROR_DOMAIN_H
#define DIAG_UDS_NRC_ERROR_DOMAIN_H

#include <stdint.h>
#include <exception>
#include "../core/error_code.h"

namespace ara
{
    namespace diag
    {
        enum class DiagUdsNrcErrc : int32_t
        {
            kGeneralReject = 0x10,
            kServiceNotSupported = 0x11,
            kSubfunctionNotSupported = 0x12,
            kIncorrectMessageLengthOrInvalidFormat = 0x13,
            kResponseTooLong = 0x14,
            kBusyRepeatRequest = 0x21,
            kConditionsNotCorrect = 0x22,
            kRequestSequenceError = 0x24,
            kNoResponseFromSubnetComponent = 0x25,
            kFailurePreventsExecutionOfRequestedAction = 0x26,
            kRequestOutOfRange = 0x31,
            kSecurityAccessDenied = 0x33,
            kInvalidKey = 0x35,
            kExceedNumberOfAttempts = 0x36,
            kRequiredTimeDelayNotExpired = 0x37,
            kUploadDownloadNotAccepted = 0x70,
            kTransferDataSuspended = 0x71,
            kGeneralProgrammingFailure = 0x72,
            kWrongBlockSequenceCounter = 0x73,
            kSubFunctionNotSupportedInActiveSession = 0x7E,
            kServiceNotSupportedInActiveSession = 0x7F,
            kRpmTooHigh = 0x81,
            kRpmTooLow = 0x82,
            kEngineIsRunning = 0x83,
            kEngineIsNotRunning = 0x84,
            kEngineRunTimeTooLow = 0x85,
            kTemperatureTooHigh = 0x86,
            kTemperatureTooLow = 0x87,
            kVehicleSpeedTooHigh = 0x88,
            kVehicleSpeedTooLow = 0x89,
            kThrottlePedalTooHigh = 0x8A,
            kThrottlePedalTooLow = 0x8B,
            kTransmissionRangeNotInNeutral = 0x8C,
            kTransmissionRangeNotInGear = 0x8D,
            kBrakeSwitchNotClosed = 0x8F,
            kShifterLeverNotInPark = 0x90,
            kTorqueConverterClutchLocked = 0x91,
            kVoltageTooHigh = 0x92,
            kVoltageTooLow = 0x93,
            kResourceTemporarilyNotAvailable = 0x94,
            kNoProcessingNoResponse = 0xFF
        };

        class DiagUdsNrcException : public std::exception
        {
        public:
            explicit DiagUdsNrcException(ara::core::ErrorCode err) noexcept;
        };

        class DiagUdsNrcErrorDomain final : public ara::core::ErrorDomain
        {
        public:
            constexpr DiagUdsNrcErrorDomain() noexcept;
            const char *Name() const noexcept override;
            const char *Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override;
            void ThrowAsException(const ara::core::ErrorCode &errorCode) const noexcept(false) override;
            constexpr const ara::core::ErrorDomain &GetDiagUdsNrcDomain() noexcept;
            constexpr ara::core::ErrorCode MakeErrorCode(DiagUdsNrcErrc code) noexcept;
        };
    }
}

#endif