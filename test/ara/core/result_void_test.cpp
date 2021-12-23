#include <gtest/gtest.h>
#include "../../../src/ara/core/result.h"

namespace ara
{
    namespace core
    {
        TEST(ResultVoidTest, DefaultConstructor)
        {
            Result<void, int> _result;
            EXPECT_TRUE(_result.HasValue());
        }

        TEST(ResultVoidTest, CopyErrorConstructor)
        {
            const int cError{1};
            Result<void, int> _result{cError};

            EXPECT_EQ(cError, _result.Error());
        }

        TEST(ResultVoidTest, MoveErrorConstructor)
        {
            const int cError{1};
            int _error{cError};
            Result<void, int> _result{std::move(_error)};

            EXPECT_EQ(cError, _result.Error());
        }

        TEST(ResultVoidTest, CopyConstructor)
        {
            Result<void, int> _errorResult{1};
            Result<void, int> _copiedErrorResult{_errorResult};

            EXPECT_EQ(_errorResult.Error(), _copiedErrorResult.Error());
        }

        TEST(ResultVoidTest, MoveConstructor)
        {
            const int cError{1};
            Result<void, int> _errorResult{cError};
            Result<void, int> _movedErrorResult{std::move(_errorResult)};

            EXPECT_EQ(cError, _movedErrorResult.Error());
        }

        TEST(ResultVoidTest, SwapMethod)
        {
            const int cError{1};
            const int cOtherError{0};

            Result<void, int> _result{cError};
            Result<void, int> _otherResult;

            _result.Swap(_otherResult);
            EXPECT_EQ(cError, _otherResult.Error());

            // Revert swapping
            _result.Swap(_otherResult);
            EXPECT_EQ(cError, _result.Error());

            int _otherError{cOtherError};
            _otherResult.EmplaceError(std::move(_otherError));
            _result.Swap(_otherResult);
            EXPECT_EQ(cOtherError, _result.Error());
            EXPECT_EQ(cError, _otherResult.Error());
        }

        TEST(ResultVoidTest, ErrorProperty)
        {
            Result<void, int> _result;
            EXPECT_THROW(_result.Error(), std::runtime_error);
        }

        TEST(ResultVoidTest, ErrMethod)
        {
            Result<void, int> _result{0};
            auto _optional{_result.Err()};

            EXPECT_TRUE(_optional.HasValue());
        }

        TEST(ResultVoidTest, ErrorOrMethod)
        {
            const int cError{1};
            Result<void, int> _result;

            EXPECT_EQ(cError, _result.ErrorOr(cError));
        }

        TEST(ResultVoidTest, CheckErrorMethod)
        {
            const int cError{1};
            Result<void, int> _result;

            EXPECT_FALSE(_result.CheckError(cError));

            int _error{cError};
            _result.EmplaceError(std::move(_error));

            EXPECT_TRUE(_result.CheckError(cError));
        }

        TEST(ResultVoidTest, BindMethod)
        {
            const char *cTrueStr = "true";

            Result<void, int> _result;
            auto _callable = [cTrueStr]()
            { return cTrueStr; };

            EXPECT_STREQ(cTrueStr, _result.Bind(_callable).Value());
        }

        TEST(ResultVoidTest, Eqaulity)
        {
            const Result<void, int> cResult;
            const Result<void, int> cOtherResult;

            EXPECT_TRUE(cResult == cOtherResult);
        }

        TEST(ResultVoidTest, ErrorEquality)
        {
            const int cError{1};
            const Result<void, int> cResult{cError};
            const Result<void, int> cOtherResult;

            EXPECT_FALSE(cResult == cOtherResult);
            EXPECT_TRUE(cResult == cError);
            EXPECT_FALSE(cError == cOtherResult);
        }

        TEST(ResultVoidTest, Ineqaulity)
        {
            const Result<void, int> cResult;
            const Result<void, int> cOtherResult;

            EXPECT_FALSE(cResult != cOtherResult);
        }

        TEST(ResultVoidTest, ErrorInequality)
        {
            const int cError{1};
            const Result<void, int> cResult{cError};
            const Result<void, int> cOtherResult;

            EXPECT_TRUE(cResult != cOtherResult);
            EXPECT_FALSE(cResult != cError);
            EXPECT_TRUE(cError != cOtherResult);
        }
    }
}