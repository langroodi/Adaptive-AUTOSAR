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
            /// @brief Application logging sink
            class LogSink
            {
            private:
                std::string mApplicationId;
                std::string mApplicationDescription;

            protected:
                /// @brief Whitespace constant character
                const std::string cWhitespace{" "};

                /// @brief Constructor
                /// @param appId Application ID
                /// @param appDescription Application description
                LogSink(std::string appId, std::string appDescription);

                /// @brief Get the application stamp (application ID and description)
                /// @returns A log stream with the application stamp
                LogStream GetAppstamp() const;

                /// @brief Get the current timestamp at the function call
                /// @brief A log stream with the current long format date-time
                LogStream GetTimestamp() const;

            public:
                virtual ~LogSink() noexcept = default;

                /// @brief Log a stream corresponds to the current application
                /// @param logStream Input log stream
                virtual void Log(const LogStream &logStream) const = 0;
            };
        }
    }
}

#endif