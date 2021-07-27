#include <stdexcept>
#include "./logger.h"

namespace ara
{
    namespace log
    {
        class LoggingFramework
        {
        private:
            std::string mApplicationId;
            std::string mApplicationDescription;
            std::string mFilePath;
            LogMode mLogMode;
            LogLevel mDefaultLogLevel;

        public:
            LoggingFramework(
                std::string appId,
                LogMode logMode,
                LogLevel logLevel = LogLevel::kWarn,
                std::string appDescription = "");
            LoggingFramework(
                std::string appId,
                std::string filePath,
                LogLevel logLevel = LogLevel::kWarn,
                std::string appDescription = "");
            ~LoggingFramework() noexcept;
            Logger &CreateLogger(
                std::string ctxId,
                std::string ctxDescription) noexcept;
            Logger &CreateLogger(
                std::string ctxId,
                std::string ctxDescription,
                LogLevel ctxDefLogLevel) noexcept;
        };
    }
}