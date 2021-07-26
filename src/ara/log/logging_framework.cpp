#include "./logging_framework.h"

namespace ara
{
    namespace log
    {
        LoggingFramework::LoggingFramework(
            std::string appId,
            LogMode logMode,
            LogLevel logLevel = LogLevel::kWarn,
            std::string appDescription = "") : mApplicationId{appId},
                                               mLogMode{logMode},
                                               mDefaultLogLevel{logLevel},
                                               mApplicationDescription{appDescription}
        {
            if (mLogMode == LogMode::kFile)
            {
                throw std::invalid_argument(
                    "File logging mode is not feasible within this constructor override.");
            }
        }

        LoggingFramework::LoggingFramework(
            std::string appId,
            std::string filePath,
            LogLevel logLevel = LogLevel::kWarn,
            std::string appDescription = "") : mApplicationId{appId},
                                               mLogMode{LogMode::kFile},
                                               mFilePath{filePath},
                                               mDefaultLogLevel{logLevel},
                                               mApplicationDescription{appDescription}
        {
        }
    }
}