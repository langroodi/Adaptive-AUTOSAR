#include <gtest/gtest.h>
#include "../../../src/ara/core/result.h"

namespace ara
{
    namespace core
    {
        TEST(ResultTest, CopyValueConstructor)
        {
            const bool cValue{true};
            Result<bool, int> _result{cValue};

            EXPECT_TRUE(_result.HasValue());
            EXPECT_EQ(cValue, _result.Value());
        }

        TEST(ResultTest, MoveValueConstructor)
        {
            const bool cValue{true};
            bool _value{cValue};
            Result<bool, int> _result{std::move(_value)};

            EXPECT_TRUE(_result.HasValue());
            EXPECT_EQ(cValue, _result.Value());
        }

        TEST(ResultTest, CopyErrorConstructor)
        {
            const int cError{1};
            Result<bool, int> _result{cError};

            EXPECT_FALSE(_result.HasValue());
            EXPECT_EQ(cError, _result.Error());
        }

        TEST(ResultTest, MoveErrorConstructor)
        {
            const int cError{1};
            int _error{cError};
            Result<bool, int> _result{std::move(_error)};

            EXPECT_FALSE(_result.HasValue());
            EXPECT_EQ(cError, _result.Error());
        }

        TEST(ResultTest, CopyConstructor)
        {
            Result<bool, int> _valueResult{true};
            Result<bool, int> _copiedValueResult{_valueResult};

            EXPECT_EQ(_valueResult.Value(), _copiedValueResult.Value());

            Result<bool, int> _errorResult{1};
            Result<bool, int> _copiedErrorResult{_errorResult};

            EXPECT_EQ(_errorResult.Error(), _copiedErrorResult.Error());
        }

        TEST(ResultTest, MoveConstructor)
        {
            const bool cValue{true};
            Result<bool, int> _valueResult{cValue};
            Result<bool, int> _movedValueResult{std::move(_valueResult)};

            EXPECT_EQ(cValue, _movedValueResult.Value());

            const int cError{1};
            Result<bool, int> _errorResult{cError};
            Result<bool, int> _movedErrorResult{std::move(_errorResult)};

            EXPECT_EQ(cError, _movedErrorResult.Error());
        }

        TEST(ResultTest, SwapMethod)
        {
            const bool cValue{true};
            const bool cOtherValue{false};
            const int cError{1};
            const int cOtherError{0};

            Result<bool, int> _result{cValue};
            Result<bool, int> _otherResult{cOtherValue};

            _result.Swap(_otherResult);
            EXPECT_EQ(cOtherValue, _result.Value());
            EXPECT_EQ(cValue, _otherResult.Value());

            int _error{cError};
            _result.EmplaceError(std::move(_error));
            _result.Swap(_otherResult);
            EXPECT_TRUE(_result.HasValue());
            EXPECT_EQ(cValue, _result.Value());
            EXPECT_FALSE(_otherResult.HasValue());
            EXPECT_EQ(cError, _otherResult.Error());

            // Revert swapping
            _result.Swap(_otherResult);
            EXPECT_FALSE(_result.HasValue());
            EXPECT_EQ(cError, _result.Error());
            EXPECT_TRUE(_otherResult.HasValue());
            EXPECT_EQ(cValue, _otherResult.Value());

            int _otherError{cOtherError};
            _otherResult.EmplaceError(std::move(_otherError));
            _result.Swap(_otherResult);
            EXPECT_EQ(cOtherError, _result.Error());
            EXPECT_EQ(cError, _otherResult.Error());
        }

        TEST(ResultTest, ValueProperty)
        {
            Result<bool, int> _result{0};
            EXPECT_THROW(_result.Value(), std::runtime_error);
        }

        TEST(ResultTest, ErrorProperty)
        {
            Result<bool, int> _result{false};
            EXPECT_THROW(_result.Error(), std::runtime_error);
        }

        TEST(ResultTest, OkMethod)
        {
            Result<bool, int> _result{false};
            auto _optional{_result.Ok()};

            EXPECT_TRUE(_optional.HasValue());
        }

        TEST(ResultTest, ErrMethod)
        {
            Result<bool, int> _result{0};
            auto _optional{_result.Err()};

            EXPECT_TRUE(_optional.HasValue());
        }

        TEST(ResultTest, ValueOrMethod)
        {
            const bool cValue{true};
            Result<bool, int> _result{1};

            EXPECT_EQ(cValue, _result.ValueOr(cValue));
        }

        TEST(ResultTest, ErrorOrMethod)
        {
            const int cError{1};
            Result<bool, int> _result{true};

            EXPECT_EQ(cError, _result.ErrorOr(cError));
        }

        TEST(ResultTest, CheckErrorMethod)
        {
            const int cError{1};
            Result<bool, int> _result{true};

            EXPECT_FALSE(_result.CheckError(cError));

            int _error{cError};
            _result.EmplaceError(std::move(_error));

            EXPECT_TRUE(_result.CheckError(cError));
        }

        TEST(ResultTest, ResolveMethod)
        {
            Result<bool, int> _result{1};
            auto _callable = [](int error)
            { return error > 0; };

            EXPECT_TRUE(_result.Resolve(_callable));
        }

        TEST(ResultTest, BindMethod)
        {
            const char *cTrueStr = "true";
            const char *cFalseStr = "false";

            Result<bool, int> _result{true};
            auto _callable = [cTrueStr, cFalseStr](bool value)
            { return value ? cTrueStr : cFalseStr; };

            EXPECT_STREQ(cTrueStr, _result.Bind(_callable).Value());
        }

        TEST(ResultTest, ValueEquality)
        {
            const bool cValue{true};
            const Result<bool, int> cResult{cValue};
            const Result<bool, int> cOtherResult{cValue};

            EXPECT_TRUE(cResult == cOtherResult);
            EXPECT_TRUE(cResult == cValue);
            EXPECT_TRUE(cValue == cOtherResult);
        }

        TEST(ResultTest, ErrorEquality)
        {
            const int cError{1};
            const Result<bool, int> cResult{cError};
            const Result<bool, int> cOtherResult{cError};

            EXPECT_TRUE(cResult == cOtherResult);
            EXPECT_TRUE(cResult == cError);
            EXPECT_TRUE(cError == cOtherResult);
        }

        TEST(ResultTest, ValueInequality)
        {
            const bool cValue{true};
            const Result<bool, int> cResult{cValue};
            const Result<bool, int> cOtherResult{cValue};

            EXPECT_FALSE(cResult != cOtherResult);
            EXPECT_FALSE(cResult != cValue);
            EXPECT_FALSE(cValue != cOtherResult);
        }

        TEST(ResultTest, ErrorInequality)
        {
            const int cError{1};
            const Result<bool, int> cResult{cError};
            const Result<bool, int> cOtherResult{cError};

            EXPECT_FALSE(cResult != cOtherResult);
            EXPECT_FALSE(cResult != cError);
            EXPECT_FALSE(cError != cOtherResult);
        }
    }
}