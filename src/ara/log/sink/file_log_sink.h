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
                FileLogSink() = delete;
                FileLogSink(
                    std::string appId,
                    std::string appDescription,
                    std::string logFilePath);
                void Log(const LogStream &logStream) const override;
            };
        }
    }
}

#endif