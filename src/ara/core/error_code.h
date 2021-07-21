#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include "error_domain.h"
#include <string>

namespace ara
{
    namespace core
    {
        class ErrorCode final
        {
        public:
            template <typename EnumT>
            constexpr ErrorCode(EnumT e) noexcept;
            constexpr ErrorCode(ErrorDomain::CodeType value, ErrorDomain const &domain) noexcept;
            constexpr ErrorDomain::CodeType Value() const noexcept;
            constexpr ErrorDomain const &Domain() const noexcept;
            std::string Message() const noexcept;
            void ThrowAsException() const noexcept(false);
            constexpr bool operator==(const ErrorCode &other) noexcept;
            constexpr bool operator!=(const ErrorCode &other) noexcept;
            std::string ToString() const;
        };
    }
}

#endif