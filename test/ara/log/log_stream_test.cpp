#include <gtest/gtest.h>
#include "../../../src/ara/log/log_stream.h"

namespace ara
{
    namespace log
    {
        TEST(LogStreamTest, FlushMethod)
        {
            const std::string cLevelString = "Log level: ";
            const LogLevel cLevel = LogLevel::kDebug;
            LogStream _levelStream;
            _levelStream << cLevelString << cLevel;

            const bool cActive = true;
            LogStream _activeStream;
            _activeStream << "; Active: " << cActive;

            LogStream _logStream;
            _logStream << _levelStream << _activeStream;

            const std::string cExpectedResult =
                "Log level: Debug; Active: true";
            std::string _actualResult = _logStream.ToString();

            EXPECT_EQ(cExpectedResult, _actualResult);

            _logStream.Flush();
            _actualResult = _logStream.ToString();
            bool _isEmpty = _actualResult.empty();

            EXPECT_TRUE(_isEmpty);
        }

        TEST(LogStreamTest, ByteInsertionOperator)
        {
            const uint8_t cByte = 128;
            LogStream _logStream;
            _logStream << cByte;

            const std::string cExpectedResult = "128";
            std::string _actualResult = _logStream.ToString();

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST(LogStreamTest, ByteVectorInsertionOperator)
        {
            std::vector<uint8_t> _byteVector{10, 20, 30};
            LogStream _logStream;
            _logStream << _byteVector;

            const std::string cExpectedResult = "0a141e";
            std::string _actualResult = _logStream.ToString();

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}