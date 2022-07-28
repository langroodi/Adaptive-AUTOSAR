#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/nrc_exception.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            TEST(NrcExceptionTest, Constructor)
            {
                const uint8_t cExpectedResult{0x13};

                NrcExecption _exception(cExpectedResult);
                uint8_t _actualResult{_exception.GetNrc()};

                EXPECT_EQ(cExpectedResult, _actualResult);
            }
        }
    }
}