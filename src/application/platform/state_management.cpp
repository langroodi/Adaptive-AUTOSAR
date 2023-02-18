#include <utility>
#include "../../ara/exec/execution_client.h"
#include "../helper/argument_configuration.h"
#include "./state_management.h"

namespace application
{
    namespace platform
    {
        const std::string StateManagement::cAppId{"StateManagement"};

        StateManagement::StateManagement(AsyncBsdSocketLib::Poller *poller) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                                                              mInstanceSpecifier{ara::core::InstanceSpecifier::Create(cAppId).Value()}
        {
        }

        helper::RpcConfiguration StateManagement::getRpcConfiguration(
            const std::string &configFilepath)
        {
            const std::string cNetworkEndpoint{"RpcServerEP"};
            const std::string cApplicationEndpoint{"ServerUnicastTcp"};

            helper::RpcConfiguration _result;
            bool _successful{
                helper::TryGetRpcConfiguration(
                    configFilepath, cNetworkEndpoint, cApplicationEndpoint,
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

        void StateManagement::configureStates(
            const ara::exec::FunctionGroup &functionGroup,
            std::string &&functionGroupContent)
        {
            const arxml::ArxmlReader cArxmlReader(
                functionGroupContent.c_str(), functionGroupContent.length());

            const arxml::ArxmlNodeRange cFunctionGroupStateNodes{
                cArxmlReader.GetNodes(
                    {"FUNCTION-GROUP", "MODE-DECLARATION-GROUP", "MODE-DECLARATIONS"})};

            for (const auto cFunctionGroupStateNode : cFunctionGroupStateNodes)
            {
                std::string _shortName{cFunctionGroupStateNode.GetShortName()};

                mFunctionGroupStates.push_back(
                    std::move(
                        ara::exec::FunctionGroupState::Create(
                            functionGroup, _shortName)
                            .Value()));

                ara::log::LogStream _logStream;
                std::string _functionGroupInstance{
                    functionGroup.GetInstance().ToString()};
                _logStream << "State: " << _shortName << " of function group: " << _functionGroupInstance << " is configured.";
                Log(cLogLevel, _logStream);
            }
        }

        void StateManagement::configureFunctionGroups(const std::string &configFilepath)
        {
            const arxml::ArxmlReader cArxmlReader(configFilepath);

            const arxml::ArxmlNodeRange cFunctionGroupNodes{
                cArxmlReader.GetNodes(
                    {"AUTOSAR", "AR-PACKAGES", "AR-PACKAGE", "ELEMENTS", "FUNCTION-GROUPS"})};

            for (const auto cFunctionGroupNode : cFunctionGroupNodes)
            {
                std::string _shortName{cFunctionGroupNode.GetShortName()};

                mFunctionGroups.push_back(
                    std::move(ara::exec::FunctionGroup::Create(_shortName).Value()));

                ara::log::LogStream _logStream;
                _logStream << "Function group: " << _shortName << " is configured.";
                Log(cLogLevel, _logStream);

                std::string _nodeContent{cFunctionGroupNode.GetContent()};
                configureStates(mFunctionGroups.back(), std::move(_nodeContent));
            }
        }

        void StateManagement::onUndefinedState(
            const ara::exec::ExecutionErrorEvent &event)
        {
            ara::log::LogStream _logStream;
            _logStream
                << "Function group: "
                << event.functionGroup->GetInstance().ToString()
                << " has undefined state because of error code "
                << event.executionError;
            Log(cLogLevel, _logStream);
        }

        void StateManagement::reportExecutionState(
            ara::com::someip::rpc::RpcClient *rpcClient)
        {
            const auto cExecutionState{ara::exec::ExecutionState::kRunning};

            ara::exec::ExecutionClient _executionClient(
                mInstanceSpecifier, rpcClient);

            ara::core::Result<void> _result{
                _executionClient.ReportExecutionState(cExecutionState)};

            if (!_result.HasValue())
            {
                std::string _errorMessage{_result.Error().Message()};
                throw std::runtime_error(std::move(_errorMessage));
            }
        }

        std::shared_future<void> StateManagement::transitToStartUpState(
            ara::exec::StateClient &stateClient)
        {
            const std::string cFunctionGroup{"MachineFG"};
            const std::string cState{"StartUp"};

            for (auto &functionGroupState : mFunctionGroupStates)
            {
                if (functionGroupState.GetFunctionGroup().GetInstance().ToString() == cFunctionGroup &&
                    functionGroupState.GetState() == cState)
                {
                    return stateClient.SetState(functionGroupState);
                }
            }

            std::string cErrorMessage{
                "State: " + cState + " for function group " + cFunctionGroup + " cannot be found."};
            throw std::runtime_error(cErrorMessage);
        }

        int StateManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const std::string cConfigArgument{helper::ArgumentConfiguration::cConfigArgument};

            ara::log::LogStream _logStream;

            try
            {
                const std::string cConfigFilepath{arguments.at(cConfigArgument)};

                const helper::RpcConfiguration cRpcConfiguration{
                    getRpcConfiguration(cConfigFilepath)};
                ara::com::someip::rpc::SocketRpcClient _rpcClient(
                    Poller,
                    cRpcConfiguration.ipAddress,
                    cRpcConfiguration.portNumber,
                    cRpcConfiguration.protocolVersion);

                configureFunctionGroups(cConfigFilepath);

                auto _undefinedStateCallback(
                    std::bind(
                        &StateManagement::onUndefinedState,
                        this, std::placeholders::_1));
                ara::exec::StateClient _stateClient(
                    _undefinedStateCallback, &_rpcClient);

                std::future<void> _executionStateReport{
                    std::async(
                        std::launch::async,
                        &StateManagement::reportExecutionState,
                        this,
                        &_rpcClient)};

                std::shared_future<void> _initialMachineStateTransition{
                    _stateClient.GetInitialMachineStateTransitionResult()};
                std::shared_future<void> _startUpStateTransition{
                    transitToStartUpState(_stateClient)};

                _logStream << "State management has been initialized.";
                Log(cLogLevel, _logStream);

                bool _running{true};

                while (!cancellationToken->load() && _running)
                {
                    _running = WaitForActivation();

                    checkFuture(
                        _executionStateReport,
                        "Execution state is reported successfully.");
                    checkFuture(
                        _initialMachineStateTransition,
                        "EM initial machine state transition is fetched successfully.");
                    checkFuture(
                        _startUpStateTransition,
                        "EM is transited to the start-up state successfully.");
                }

                _logStream.Flush();
                _logStream << "State management has been terminated.";
                Log(cLogLevel, _logStream);

                return cSuccessfulExitCode;
            }
            catch (const std::runtime_error &ex)
            {
                _logStream.Flush();
                _logStream << ex.what();
                Log(cErrorLevel, _logStream);

                return cUnsuccessfulExitCode;
            }
        }
    }
}