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
                LogStream _appstamp = GetAppstamp();
                LogStream _timestamp = GetTimestamp();
                _appstamp << _timestamp << cWhitespace << logStream;
                std::string _logString = _appstamp.ToString();

                std::cout << _logString << std::endl;
            }
        }
    }
}