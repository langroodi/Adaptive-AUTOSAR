#ifndef ERROR_CODE_DOMAIN_H
#define ERROR_CODE_DOMAIN_H

#include "../core/error_code.h"
#include "./exec_error_domain.h"

namespace ara
{
    namespace exec
    {
        /// @brief Execution Management exception
        class ExecException : public std::exception
        {
        private:
            core::ErrorCode mErrorCode;

        public:
            ExecException() = delete;
            ~ExecException() noexcept = default;

            /// @brief Constructor
            /// @param errorCode Exception error code in the current error domain
            explicit ExecException(core::ErrorCode errorCode) noexcept;

            /// @brief Get the exception error domain
            /// @returns EM error domain
            const core::ErrorDomain &GetExecErrorDomain() noexcept;

            /// @brief Make a error code in the same error domain
            /// @param code EM error code number
            /// @returns Created error code with determined error number
            core::ErrorCode MakeErrorCode(ExecErrc code) noexcept;

            /// @brief Get the error code
            /// @return Exception error code reference
            /// @note This is not an ARA specified property.
            const core::ErrorCode &GetErrorCode() const noexcept;
        };
    }
}

#endif