#include <gtest/gtest.h>
#include "../../../../src/ara/exec/helper/atomic_optional.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            TEST(AtomicOptionalTest, DefaultConstructor)
            {
                AtomicOptional<uint64_t> _atomicOptional;
                EXPECT_FALSE(_atomicOptional.HasValue());
            }

            TEST(AtomicOptionalTest, Constructor)
            {
                const uint64_t cValue{1};
                AtomicOptional<uint64_t> _atomicOptional{cValue};

                EXPECT_EQ(cValue, _atomicOptional.Value());
            }

            TEST(AtomicOptionalTest, AssignmentOperator)
            {
                const uint64_t cValue{1};
                AtomicOptional<uint64_t> _atomicOptional;
                _atomicOptional = cValue;

                EXPECT_EQ(cValue, _atomicOptional.Value());
            }

            TEST(AtomicOptionalTest, ResetMethod)
            {
                const uint64_t cValue{1};
                AtomicOptional<uint64_t> _atomicOptional{cValue};
                EXPECT_TRUE(_atomicOptional.HasValue());
                
                _atomicOptional.Reset();
                EXPECT_FALSE(_atomicOptional.HasValue());
            }
        }
    }
}