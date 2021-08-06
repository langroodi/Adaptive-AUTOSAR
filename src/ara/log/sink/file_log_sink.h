#ifndef FILE_LOG_SINK_H
#define FILE_LOG_SINK_H

#include <fstream>
#include "./log_sink.h"

namespace ara
{
    namespace log
    {
        namespace sink
        {
            class FileLogSink : public LogSink
            {
            private:
                std::string mLogFilePath;

            public:
                /// @brief Constructor
                /// @param appId Application ID
                /// @param appDescription Application description
                /// @param logFilePath Logging file sink path
                FileLogSink(
                    std::string appId,
                    std::string appDescription,
                    std::string logFilePath);

                FileLogSink() = delete;
                void Log(const LogStream &logStream) const override;
            };
        }
    }
}

#endif