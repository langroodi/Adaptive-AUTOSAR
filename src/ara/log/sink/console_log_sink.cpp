#include "./console_log_sink.h"

namespace ara
{
    namespace log
    {
        namespace sink
        {
            ConsoleLogSink::ConsoleLogSink(
                std::string appId,
                std::string appDescription) : LogSink(appId, appDescription)
            {
            }

            void ConsoleLogSink::Log(const LogStream &logStream) const
            {
                LogStream _timestamp = GetTimestamp();
                LogStream _appstamp = GetAppstamp();
                _timestamp << cWhitespace << _appstamp << cWhitespace << logStream;
                std::string _logString = _timestamp.ToString();

                std::cout << _logString << std::endl;
            }
        }
    }
}