#include <gtest/gtest.h>

#include "../../../src/ara/log/log_stream.h"

namespace ara
{
    namespace log
    {
                TEST(LogStreamTest, ByteInsertionOperator)
        {
            uint8_t _byte = 128;
            LogStream _logStream;
            _logStream << _byte;

            const std::string _expectedResult = "128";
            std::string _actualResult = _logStream.ToString();

            EXPECT_EQ(_expectedResult, _actualResult);
        }
    }
}