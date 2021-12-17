#ifndef ERROR_CODE_DOMAIN_H
#define ERROR_CODE_DOMAIN_H

#include <exception>
#include "../core/error_domain.h"
#include "../core/error_code.h"

namespace ara
{
    namespace exec
    {
        enum class ExecErrc : ara::core::CodeType
        {
            kGeneralError = 1,
            kInvalidArguments = 2,
            kCommunicationError = 3,
            kMetaModelError = 4,
            kCancelled = 5,
            kFailed = 6
        };

        class ExecException : public std::exception
        {
        public:
            explicit ExecException(ara::core::ErrorCode errorCode) noexcept;
            const ara::core::ErrorDomain &GetExecErrorDomain() noexcept;
            ara::core::ErrorCode MakeErrorCode(ExecErrc code) noexcept;
        };

        class ExceErrorDomain final : public ara::core::ErrorDomain
        {
        public:
            ExceErrorDomain() noexcept;
            const char *Name() const noexcept override;
            const char *Message(
                ara::core::CodeType errorCode) const noexcept override;
        };
    }
}

#endif