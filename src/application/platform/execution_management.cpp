#include "../../ara/com/someip/rpc/socket_rpc_server.h"
#include "../../ara/exec/state_server.h"
#include "./execution_management.h"

namespace application
{
    namespace platform
    {
        const std::string ExecutionManagement::cAppId{"ExecutionManagement"};
        const ara::log::LogMode ExecutionManagement::cLogMode{ara::log::LogMode::kConsole};
        const std::string ExecutionManagement::cContextId{"Lifetime"};
        const std::string ExecutionManagement::cContextDescription{"Application lifetime logs"};
        const ara::log::LogLevel ExecutionManagement::cLogLevel{ara::log::LogLevel::kInfo};
        const ara::log::LogLevel ExecutionManagement::cErrorLevel{ara::log::LogLevel::kError};

        ExecutionManagement::ExecutionManagement() : mLoggingFramework{ara::log::LoggingFramework::Create(cAppId, cLogMode)},
                                                     mLogger{mLoggingFramework->CreateLogger(cContextId, cContextDescription, cLogLevel)}
        {
        }

        helper::RpcConfiguration ExecutionManagement::getRpcConfiguration(
            const std::map<std::string, std::string> &arguments)
        {
            const std::string cConfigArgument{"config"};
            const std::string cNetworkEndpoint{"RpcServerEP"};
            const std::string cApplicationEndpoint{"ServerUnicastTcp"};

            std::string _configFilepath{arguments.at(cConfigArgument)};
            helper::RpcConfiguration _result;
            bool _successful{
                helper::TryGetCRpcConfiguration(
                    _configFilepath, cNetworkEndpoint, cApplicationEndpoint,
                    _result)};

            if (_successful)
            {
                return _result;
            }
            else
            {
                throw std::runtime_error("RPC configuration failed.");
            }
        }

        int ExecutionManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const int cSuccessfulExitCode{0};
            const int cUnsuccessfulExitCode{1};

            ara::log::LogStream _logStream;

            try
            {
                const helper::RpcConfiguration cRpcConfiguration{
                    getRpcConfiguration(arguments)};
                ara::com::someip::rpc::SocketRpcServer _rpcServer(
                    &mPoller,
                    cRpcConfiguration.ipAddress,
                    cRpcConfiguration.portNumber,
                    cRpcConfiguration.protocolVersion);

                _logStream << "Execution management has been initialized.";
                mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

                mStateManagement.Initialize(arguments);

                ara::exec::ActivationReturnType _activationReturn{
                    ara::exec::ActivationReturnType::kInit};

                while (!cancellationToken->load() &&
                       _activationReturn != ara::exec::ActivationReturnType::kTerminate)
                {
                    auto _activationReturnResult{mDeterministicClient.WaitForActivation()};
                    _activationReturn = _activationReturnResult.Value();

                    ///@todo Execution management logic here
                }

                int _result{mStateManagement.Terminate()};

                _logStream.Flush();
                _logStream << "Execution management has been terminated.";
                mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

                return _result;
            }
            catch (const std::runtime_error &ex)
            {
                _logStream.Flush();
                _logStream << ex.what();
                mLoggingFramework->Log(mLogger, cErrorLevel, _logStream);

                return cUnsuccessfulExitCode;
            }
        }

        ExecutionManagement::~ExecutionManagement()
        {
            mStateManagement.Terminate();
            delete mLoggingFramework;
        }
    }
}