#include <gtest/gtest.h>
#include "../../../src/ara/core/error_code.h"

namespace ara
{
    namespace core
    {
        class DummyErrorDomain : public ErrorDomain
        {
        public:
            DummyErrorDomain() : ErrorDomain{0}
            {
            }

            virtual const char *Name() const noexcept override
            {
                return "";
            }

            virtual const char *Message(CodeType errorCode) const noexcept override
            {
                return "";
            }
        };

        TEST(ErrorCodeTest, Constructor)
        {
            const ErrorDomain::CodeType cRawErrorCode = 1;
            DummyErrorDomain _errorDomain;
            ErrorCode _errorCode{cRawErrorCode, _errorDomain};

            EXPECT_EQ(cRawErrorCode, _errorCode.Value());
        }

        TEST(ErrorCodeTest, ThrowMethod)
        {
            const ErrorDomain::CodeType cRawErrorCode = 1;
            DummyErrorDomain _errorDomain;
            ErrorCode _errorCode{cRawErrorCode, _errorDomain};

            EXPECT_ANY_THROW(_errorCode.ThrowAsException());
        }

        TEST(ErrorCodeTest, Equality)
        {
            const ErrorDomain::CodeType cRawErrorCode1 = 1;
            const ErrorDomain::CodeType cRawErrorCode2 = 2;
            DummyErrorDomain _errorDomain;

            ErrorCode _errorCode0{cRawErrorCode1, _errorDomain};
            ErrorCode _errorCode1{cRawErrorCode1, _errorDomain};
            ErrorCode _errorCode2{cRawErrorCode2, _errorDomain};

            EXPECT_TRUE(_errorCode0 == _errorCode1);
            EXPECT_TRUE(_errorCode0 != _errorCode2);
        }
    }
}