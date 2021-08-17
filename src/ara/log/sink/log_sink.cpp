#include "./log_sink.h"

namespace ara
{
    namespace log
    {
        namespace sink
        {
            LogSink::LogSink(std::string appId, std::string appDescription) : mApplicationId{appId},
                                                                              mApplicationDescription{appDescription}
            {
            }

            LogStream LogSink::GetAppstamp() const
            {
                LogStream _result;
                _result << mApplicationId;

                if (!mApplicationDescription.empty())
                {
                    _result << cWhitespace << mApplicationDescription;
                }

                return _result;
            }

            LogStream LogSink::GetTimestamp() const
            {
                std::time_t _time = std::time(nullptr);
                std::tm *_localtime = std::localtime(&_time);
                char *_timestamp = std::asctime(_localtime);
                LogStream _result;
                _result << _timestamp;

                return _result;
            }
        }
    }
}