
#include "./exec_exception.h"

namespace ara
{
    namespace exec
    {
        ExecException::ExecException(core::ErrorCode errorCode) noexcept : mErrorCode{errorCode}
        {
        }

        const core::ErrorDomain &ExecException::GetExecErrorDomain() noexcept
        {
            return mErrorCode.Domain();
        }

        core::ErrorCode ExecException::MakeErrorCode(ExecErrc code) noexcept
        {
            auto _value{static_cast<core::ErrorDomain::CodeType>(code)};
            core::ErrorCode _result{_value, mErrorCode.Domain()};

            return _result;
        }

        const core::ErrorCode &ExecException::GetErrorCode() const noexcept
        {
            return mErrorCode;
        }
    }
}