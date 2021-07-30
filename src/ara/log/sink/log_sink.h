#ifndef LOG_SINK_H
#define LOG_SINK_H

#include <ctime>
#include "../log_stream.h"

namespace ara
{
    namespace log
    {
        namespace sink
        {
            class LogSink
            {
            private:
                std::string mApplicationId;
                std::string mApplicationDescription;

            protected:
                const std::string cWhitespace{" "};

                LogSink(std::string appId, std::string appDescription);
                LogStream GetAppstamp() const;
                LogStream GetTimestamp() const;

            public:
                virtual ~LogSink() noexcept;
                virtual void Log(const LogStream &logStream) const = 0;
            };
        }
    }
}

#endif