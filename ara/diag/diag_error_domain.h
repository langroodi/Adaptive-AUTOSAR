#ifndef DIAG_ERROR_DOMAIN_H
#define DIAG_ERROR_DOMAIN_H

#include <exception>
#include "../core/error_domain.h"
#include "../core/error_code.h"

namespace ara
{
    namespace diag
    {
        enum class DiagErrc : ara::core::ErrorDomain::CodeType
        {
            kAlreadyOffered = 101,
            kConfigurationMismatch = 102,
            kDebouncingConfigurationInconsistent = 103,
            kReportIgnored = 104,
            kInvalidArgument = 105,
            kNotOffered = 106,
            kGenericError = 107,
            kNoSuchDTC = 108,
            kBusy = 109,
            kFailed = 110,
            kMemoryError = 111,
            kWrongDtc = 112,
            kRejected = 113,
            kResetTypeNotSupported = 114,
            kRequestFailed = 115
        };

        class DiagException : public std::exception
        {
        public:
            explicit DiagException(ara::core::ErrorCode err) noexcept;
        };

        class DiagErrorDomain final : public ara::core::ErrorDomain
        {
        public:
            constexpr DiagErrorDomain() noexcept;
            const char *Name() const noexcept override;
            const char *Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override;
            void ThrowAsException(const ara::core::ErrorCode &errorCode) const noexcept(false) override;
            constexpr const ara::core::ErrorDomain &GetDiagDomain() noexcept;
            constexpr ara::core::ErrorCode MakeErrorCode(DiagErrc code) noexcept;
        };

        enum class DiagOfferErrc : ara::core::ErrorDomain::CodeType
        {
            kAlreadyOffered = 101,
            kConfigurationMismatch = 102,
            kDebouncingConfigurationInconsistent = 103
        };

        class DiagOfferErrorDomain : public ara::core::ErrorDomain
        {
        public:
            constexpr DiagOfferErrorDomain() noexcept;
            const char *Name() const noexcept override;
            const char *Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override;
            void ThrowAsException(const ara::core::ErrorCode &errorCode) const noexcept(false) override;
            constexpr ara::core::ErrorCode MakeErrorCode(DiagOfferErrc code) noexcept;
        };

        enum class DiagReportingErrc : ara::core::ErrorDomain::CodeType
        {
            kAlreadyOffered = 101,
            kConfigurationMismatch = 102,
            kDebouncingConfigurationInconsistent = 103,
            kReportIgnored = 104,
            kInvalidArgument = 105,
            kNotOffered = 106,
            kGenericError = 107
        };

        class DiagReportingErrorDomain : public ara::core::ErrorDomain
        {
        public:
            constexpr DiagReportingErrorDomain() noexcept;
            const char *Name() const noexcept override;
            const char *Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override;
            void ThrowAsException(const ara::core::ErrorCode &errorCode) const noexcept(false) override;
            constexpr ara::core::ErrorCode MakeErrorCode(DiagReportingErrc code) noexcept;
        };
    }
}

#endif