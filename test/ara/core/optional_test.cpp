#include <gtest/gtest.h>
#include "../../../src/ara/core/optional.h"

namespace ara
{
    namespace core
    {
        TEST(OptionalTest, DefaultConstructor)
        {
            Optional<bool> _optional;
            EXPECT_FALSE(_optional.HasValue());
        }

        TEST(OptionalTest, CopyValueConstructor)
        {
            const bool cValue = false;
            Optional<bool> _optional{cValue};

            EXPECT_TRUE(_optional.HasValue());
        }

        TEST(OptionalTest, MoveValueConstructor)
        {
            Optional<bool> _optional{false};
            EXPECT_TRUE(_optional.HasValue());
        }

        TEST(OptionalTest, CopyConstructor)
        {
            Optional<bool> _optional{true};
            Optional<bool> _copiedOptional{_optional};

            EXPECT_EQ(_optional.Value(), _copiedOptional.Value());
        }

        TEST(OptionalTest, MoveConstructor)
        {
            const bool cValue{true};
            Optional<bool> _optional{cValue};
            Optional<bool> _movedOptional{std::move(_optional)};

            EXPECT_EQ(cValue, _movedOptional.Value());
        }

        TEST(OptionalTest, CopyAssignment)
        {
            Optional<bool> _optional{true};
            Optional<bool> _copiedOptional = _optional;

            EXPECT_EQ(_optional.Value(), _copiedOptional.Value());
        }

        TEST(OptionalTest, MoveAssignment)
        {
            const bool cValue{true};
            Optional<bool> _optional{cValue};
            Optional<bool> _movedOptional = std::move(_optional);

            EXPECT_EQ(cValue, _movedOptional.Value());
        }

        TEST(OptionalTest, EmplaceMethod)
        {
            const bool cValue{true};
            Optional<bool> _optional;

            bool _value{cValue};
            _optional.Emplace(std::move(_value));

            EXPECT_TRUE(_optional.HasValue());
            EXPECT_EQ(cValue, _optional.Value());
        }

        TEST(OptionalTest, SwapMethod)
        {
            const bool cValue{true};
            const bool cOtherValue{false};

            Optional<bool> _optional{cValue};
            Optional<bool> _otherOptional;

            _optional.Swap(_otherOptional);

            EXPECT_FALSE(_optional.HasValue());
            EXPECT_TRUE(_otherOptional.HasValue());
            EXPECT_EQ(cValue, _otherOptional.Value());

            // Revert swapping
            _optional.Swap(_otherOptional);
            EXPECT_TRUE(_optional.HasValue());
            EXPECT_FALSE(_otherOptional.HasValue());
            EXPECT_EQ(cValue, _optional.Value());

            bool _otherValue{cOtherValue};
            _otherOptional.Emplace(std::move(_otherValue));
            _otherOptional.Swap(_optional);

            EXPECT_TRUE(_optional.HasValue());
            EXPECT_EQ(cOtherValue, _optional.Value());
            EXPECT_TRUE(_otherOptional.HasValue());
            EXPECT_EQ(cValue, _otherOptional.Value());
        }

        TEST(OptionalTest, ResetMethod)
        {
            Optional<bool> _optional{true};
            _optional.Reset();

            EXPECT_FALSE(_optional.HasValue());
        }

        TEST(OptionalTest, BoolOperator)
        {
            Optional<bool> _optional{false};
            EXPECT_TRUE(_optional);
        }

        TEST(OptionalTest, ValueProperty)
        {
            Optional<bool> _optional;
            EXPECT_THROW(_optional.Value(), std::runtime_error);
        }

        TEST(OptionalTest, ValueOrMethod)
        {
            Optional<bool> _optional;
            EXPECT_TRUE(_optional.ValueOr<int>(1));
        }

        TEST(OptionalTest, EqualityOperator)
        {
            Optional<bool> _optional;
            Optional<bool> _otherOptional;

            EXPECT_TRUE(_optional == _otherOptional);

            _optional.Emplace(true);
            EXPECT_FALSE(_optional == _otherOptional);

            _otherOptional.Emplace(false);
            EXPECT_FALSE(_optional == _otherOptional);

            _otherOptional.Emplace(true);
            EXPECT_TRUE(_optional == _otherOptional);
        }

        TEST(OptionalTest, InequalityOperator)
        {
            Optional<bool> _optional;
            Optional<bool> _otherOptional;

            EXPECT_FALSE(_optional != _otherOptional);

            _optional.Emplace(true);
            EXPECT_TRUE(_optional != _otherOptional);

            _otherOptional.Emplace(false);
            EXPECT_TRUE(_optional != _otherOptional);

            _otherOptional.Emplace(true);
            EXPECT_FALSE(_optional != _otherOptional);
        }
    }
}