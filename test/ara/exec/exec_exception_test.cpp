#include <gtest/gtest.h>
#include "../../../src/ara/exec/exec_exception.h"

namespace ara
{
    namespace exec
    {
        TEST(ExecExceptionTest, GetExecErrorDomainMethod)
        {
            ExecErrorDomain _errorDomain;
            ExecErrc _code{ExecErrc::kGeneralError};
            auto _value{static_cast<core::ErrorDomain::CodeType>(_code)};
            core::ErrorCode _errorCode{_value, _errorDomain};
            ExecException _exception{_errorCode};

            EXPECT_TRUE(_exception.GetExecErrorDomain() == _errorDomain);
        }

        TEST(ExecExceptionTest, MakeErrorCodeMethod)
        {
            ExecErrorDomain _errorDomain;

            ExecErrc _genericCode{ExecErrc::kGeneralError};
            auto _genericValue{static_cast<core::ErrorDomain::CodeType>(_genericCode)};
            core::ErrorCode _genericErrorCode{_genericValue, _errorDomain};
            ExecException _exception{_genericErrorCode};

            ExecErrc _failedCode{ExecErrc::kFailed};
            auto _failedErrorCode = _exception.MakeErrorCode(_failedCode);
            auto _failedValue{static_cast<core::ErrorDomain::CodeType>(_failedCode)};

            EXPECT_TRUE(_failedErrorCode.Value() == _failedValue);
            EXPECT_TRUE(_failedErrorCode.Domain() == _errorDomain);
        }

        TEST(ExecExceptionTest, ErrorCodeProperty)
        {
            const ExecErrorDomain cErrorDomain;
            const ExecErrc cCode{ExecErrc::kGeneralError};
            auto _value{static_cast<core::ErrorDomain::CodeType>(cCode)};
            core::ErrorCode _errorCode(_value, cErrorDomain);
            ExecException _exception(_errorCode);

            EXPECT_TRUE(_exception.GetErrorCode() == _errorCode);
        }
    }
}