#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <string>
#include "./error_domain.h"

namespace ara
{
    namespace core
    {
        /// @brief A wrapper around the raw error code in a specific ErrorDomain
        class ErrorCode final
        {
        private:
            ErrorDomain::CodeType mValue;
            const ErrorDomain& mDomain;

        public:
            /// @brief Constructor
            /// @param value Error code value
            /// @param domain Error code domain
            constexpr ErrorCode(
                ErrorDomain::CodeType value,
                const ErrorDomain &domain) noexcept : mValue{value}, mDomain{domain}
            {
            }

            ErrorCode() = delete;
            ~ErrorCode() noexcept = default;

            /// @brief Get error code value
            /// @returns Raw error code value
            constexpr ErrorDomain::CodeType Value() const noexcept
            {
                return mValue;
            }

            /// @brief Get error code domain
            /// @returns Error domain which the error code belongs to
            constexpr ErrorDomain const &Domain() const noexcept
            {
                return mDomain;
            }

            /// @brief Get error message
            /// @returns Error code corresponding message in the defined domain
            std::string Message() const noexcept;

            /// @brief Throw the error as an exception
            void ThrowAsException() const;

            constexpr bool operator==(const ErrorCode &other) const noexcept
            {
                return mDomain == other.mDomain && mValue == other.mValue;
            }

            constexpr bool operator!=(const ErrorCode &other) const noexcept
            {
                return mDomain != other.mDomain || mValue != other.mValue;
            }
        };
    }
}

#endif