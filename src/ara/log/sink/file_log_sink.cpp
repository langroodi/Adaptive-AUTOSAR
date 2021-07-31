#include "./file_log_sink.h"

namespace ara
{
    namespace log
    {
        namespace sink
        {
            FileLogSink::FileLogSink(
                std::string appId,
                std::string appDescription,
                std::string logFilePath) : LogSink(appId, appDescription),
                                           mLogFilePath{logFilePath}
            {
            }

            void FileLogSink::Log(const LogStream &logStream) const
            {
                const std::string cNewline{"\n"};

                LogStream _timestamp = GetTimestamp();
                LogStream _appstamp = GetAppstamp();
                _timestamp << cWhitespace << _appstamp  << cWhitespace << logStream;
                std::string _logString = _timestamp.ToString();

                std::ofstream logFileStream(
                    mLogFilePath, std::ofstream::out | std::ofstream::app);
                logFileStream << _logString << cNewline;
                logFileStream.close();
            }
        }
    }
}