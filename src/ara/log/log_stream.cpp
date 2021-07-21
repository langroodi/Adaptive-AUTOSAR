#include "./log_stream.h"

namespace ara
{
    namespace log
    {
        void LogStream::concat(std::string &&log)
        {
            mLogs += log;
        }

        void LogStream::Flush() noexcept
        {
            mLogs.clear();
        }

        LogStream &LogStream::operator<<(const LogStream &value)
        {
            mLogs += value.mLogs;

            return *this;
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

        LogStream &LogStream::operator<<(LogLevel value)
        {
            switch (value)
            {
            case LogLevel::kOff:
                mLogs += "Off";
                break;
            case LogLevel::kFatal:
                mLogs += "Fatal";
                break;
            case LogLevel::kError:
                mLogs += "Error";
                break;
            case LogLevel::kWarn:
                mLogs += "Warning";
                break;
            case LogLevel::kInfo:
                mLogs += "Info";
                break;
            case LogLevel::kDebug:
                mLogs += "Debug";
                break;
            case LogLevel::kVerbose:
                mLogs += "Verbose";
                break;
            }

            return *this;
        }

        LogStream &LogStream::operator<<(const ara::core::ErrorCode &value)
        {
            std::string _valueString = value.ToString();
            concat(std::move(_valueString));

            return *this;
        }

        LogStream &LogStream::operator<<(const ara::core::InstanceSpecifier &value) noexcept
        {
            std::string _valueString = value.ToString();
            concat(std::move(_valueString));

            return *this;
        }

        LogStream &LogStream::WithLocation(std::string file, int line)
        {
            const std::string cSeparator = ":";
            std::string _lineString = std::to_string(line);
            std::string _locationString = file + cSeparator + _lineString;
            concat(std::move(_locationString));

            return *this;
        }

        std::string LogStream::ToString() const noexcept
        {
            return mLogs;
        }
    }
}
