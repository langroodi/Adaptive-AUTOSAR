#include "./log_stream.h"

namespace ara
{
    namespace log
    {
        void LogStream::Flush() noexcept
        {
            mLogs.clear();
        }

        LogStream &LogStream::operator<<(bool value)
        {
            const std::string cTrueString = "true";
            const std::string cFalseString = "false";

            std::string _valueString;
            if (value)
            {
                _valueString = cTrueString;
            }
            else
            {
                _valueString = cFalseString;
            }
            concat(std::move(_valueString));

            return *this;
        }

        LogStream &LogStream::operator<<(uint8_t value)
        {
            std::string _valueString = std::to_string(value);
            concat(std::move(_valueString));

            return *this;
        }

        LogStream &LogStream::operator<<(uint32_t value)
        {
            std::string _valueString = std::to_string(value);
            concat(std::move(_valueString));

            return *this;
        }

        LogStream &LogStream::operator<<(float value)
        {
            std::string _valueString = std::to_string(value);
            concat(std::move(_valueString));

            return *this;
        }

        LogStream &LogStream::operator<<(const std::string &value)
        {
            mLogs += value;

            return *this;
        }

        LogStream &LogStream::operator<<(const char *value)
        {
            std::string _valueString{value};
            concat(std::move(_valueString));

            return *this;
        }
    }
}
