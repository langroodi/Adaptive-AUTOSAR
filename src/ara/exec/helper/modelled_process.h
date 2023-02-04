#ifndef MODELLED_PROCESS_H
#define MODELLED_PROCESS_H

#include <asyncbsdsocket/poller.h>
#include <atomic>
#include <future>
#include <map>
#include <string>
#include "../../log/logging_framework.h"
#include "../deterministic_client.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            /// @brief A class that models an instance of an Adaptive (Platform) Application executable
            class ModelledProcess
            {
            private:
                static const log::LogMode cLogMode;
                static const std::string cContextId;
                static const std::string cContextDescription;

                log::LoggingFramework *mLoggingFramework;
                log::Logger mLogger;
                DeterministicClient mDeterministicClient;
                std::atomic_bool mCancellationToken;
                std::future<int> mExitCode;

            protected:
                /// @brief Information severity log level
                static const log::LogLevel cLogLevel;
                /// @brief Error severity log level
                static const log::LogLevel cErrorLevel;

                /// @brief Successful application exit code
                const int cSuccessfulExitCode{0};
                /// @brief Unsuccessful application exit code
                const int cUnsuccessfulExitCode{1};

                /// @brief Global poller for TCP/IP network communication
                AsyncBsdSocketLib::Poller *const Poller;

                /// @brief Constructor
                /// @param appId Modelled process application ID for logging
                /// @param poller Global poller for network communication
                ModelledProcess(
                    std::string appId, AsyncBsdSocketLib::Poller *poller);

                /// @brief Main running block of the process
                /// @param arguments Initialization arguments keys and their corresponding values
                /// @param cancellationToken Token to be for cancelling the main cycle
                /// @return Exit code
                /// @note Exit code zero means the graceful shutdown of the process.
                virtual int Main(
                    const std::atomic_bool *cancellationToken,
                    const std::map<std::string, std::string> &arguments) = 0;

                /// @brief Log a steam
                /// @param logLevel Stream log severity level
                /// @param logStream Stream to be logged
                void Log(
                    log::LogLevel logLevel, const log::LogStream &logStream);

                /// @brief Wait for the next main function activation cycle
                /// @return True if the cycle is not terminiated yet, otherwise false
                bool WaitForActivation();

            public:
                /// @brief Initialize the process model to run the main block
                /// @param arguments Initialization arguments keys and their corresponding values
                /// @see Main(const std::map<std::string, std::string> &)
                void Initialize(const std::map<std::string, std::string> &arguments);

                /// @brief Terminate the process model
                /// @return Returned exit code from the main running block
                /// @note The caller will blocked until the termination be finished.
                int Terminate();

                virtual ~ModelledProcess();
            };
        }
    }
}

#endif