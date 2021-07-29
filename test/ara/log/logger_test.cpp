#include <gtest/gtest.h>
#include <initializer_list>
#include "../../../src/ara/log/logger.h"

namespace ara
{
    namespace log
    {
        TEST(LoggerTest, LogLevelOff)
        {
            const std::string cCtxId{"CTX01"};
            const std::string cCtxDescription{"Default Test Context"};
            const LogLevel cLogLevel{LogLevel::kOff};

            Logger _logger =
                Logger::CreateLogger(cCtxId, cCtxDescription, cLogLevel);

            std::initializer_list<LogLevel>
                _logLevels{
                    LogLevel::kFatal,
                    LogLevel::kError,
                    LogLevel::kWarn,
                    LogLevel::kInfo,
                    LogLevel::kDebug,
                    LogLevel::kVerbose};

            for (auto logLevel : _logLevels)
            {
                bool _isEnabled = _logger.IsEnabled(logLevel);
                ASSERT_FALSE(_isEnabled);
            }
        }

        TEST(LoggerTest, LogLevelVerbose)
        {
            const std::string cCtxId{"CTX01"};
            const std::string cCtxDescription{"Default Test Context"};
            const LogLevel cLogLevel{LogLevel::kVerbose};

            Logger _logger =
                Logger::CreateLogger(cCtxId, cCtxDescription, cLogLevel);

            std::initializer_list<LogLevel>
                _logLevels{
                    LogLevel::kFatal,
                    LogLevel::kError,
                    LogLevel::kWarn,
                    LogLevel::kInfo,
                    LogLevel::kDebug};

            for (auto logLevel : _logLevels)
            {
                bool _isEnabled = _logger.IsEnabled(logLevel);
                ASSERT_TRUE(_isEnabled);
            }
        }

        TEST(LoggerTest, WithLevelFunction)
        {
            const std::string cCtxId{"CTX01"};
            const std::string cCtxDescription{"Default Test Context"};
            const LogLevel cLogLevel{LogLevel::kWarn};

            Logger _logger =
                Logger::CreateLogger(cCtxId, cCtxDescription, cLogLevel);

            LogStream _logStream = _logger.WithLevel(cLogLevel);
            std::string _logStreamString = _logStream.ToString();

            bool _hasId =
                _logStreamString.find(cCtxId) != std::string::npos;
            ASSERT_TRUE(_hasId);

            bool _hasDescription =
                _logStreamString.find(cCtxDescription) != std::string::npos;
            ASSERT_TRUE(_hasDescription);
        }
    }
}