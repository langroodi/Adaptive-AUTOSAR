#ifndef CONSOLE_LOG_SINK_H
#define CONSOLE_LOG_SINK_H

#include <iostream>
#include "./log_sink.h"

namespace ara
{
    namespace log
    {
        namespace sink
        {
            class ConsoleLogSink : public LogSink
            {
            public:
                ConsoleLogSink() = delete;
                ConsoleLogSink(
                    std::string appId,
                    std::string appDescription);
                void Log(const LogStream &logStream) override;
            };
        }
    }
}

#endif