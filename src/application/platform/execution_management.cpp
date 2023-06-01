#include "../../ara/com/someip/rpc/socket_rpc_server.h"
#include "../../ara/exec/execution_server.h"
#include "../helper/argument_configuration.h"
#include "./execution_management.h"

namespace application
{
    namespace platform
    {
        const std::string ExecutionManagement::cAppId{"ExecutionManagement"};
        const std::string ExecutionManagement::cFifoPath{"/tmp/fifo_communicator"};

        ExecutionManagement::ExecutionManagement(AsyncBsdSocketLib::Poller *poller) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                                                                      mCommunicator(poller, cFifoPath),
                                                                                      mStateManagement(poller),
                                                                                      mPlatformHealthManager(poller, &mCommunicator, cMachineFunctionGroup),
                                                                                      mExtendedVehicle(poller, &mCommunicator),
                                                                                      mDiagnosticManager(poller),
                                                                                      mStateServer{nullptr}
        {
        }

        helper::RpcConfiguration ExecutionManagement::getRpcConfiguration(
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

        void ExecutionManagement::fillFunctionGroupStates(
            std::string functionGroupShortName,
            const std::string &functionGroupContent,
            std::set<std::pair<std::string, std::string>> &functionGroupStates)
        {
            const arxml::ArxmlReader cArxmlReader(
                functionGroupContent.c_str(), functionGroupContent.length());

            const arxml::ArxmlNodeRange cFunctionGroupStateNodes{
                cArxmlReader.GetNodes(
                    {"FUNCTION-GROUP", "MODE-DECLARATION-GROUP", "MODE-DECLARATIONS"})};

            for (const auto cFunctionGroupStateNode : cFunctionGroupStateNodes)
            {
                std::string _stateShortName{cFunctionGroupStateNode.GetShortName()};
                functionGroupStates.emplace(functionGroupShortName, _stateShortName);
            }
        }

        void ExecutionManagement::fillInitialStates(
            std::string functionGroupShortName,
            const std::string &functionGroupContent,
            std::map<std::string, std::string> &initialStates)
        {
            const std::string cSourceNode{"INITIAL-MODE-REF"};
            const std::string cDestinationType{"MODE-DECLARATION"};

            const arxml::ArxmlReader cArxmlReader(
                functionGroupContent.c_str(), functionGroupContent.length());

            const arxml::ArxmlNode cModeDeclarationGroupNode{
                cArxmlReader.GetRootNode(
                    {"FUNCTION-GROUP", "MODE-DECLARATION-GROUP"})};

            std::string _initialStateShortName;
            bool _successful{
                cModeDeclarationGroupNode.TryGetReference(
                    cSourceNode, cDestinationType, _initialStateShortName)};
            if (_successful)
            {
                initialStates[functionGroupShortName] = _initialStateShortName;
            }
            else
            {
                const std::string cErrorMessage{
                    functionGroupShortName + " lacks an initial state."};
                throw std::runtime_error(cErrorMessage);
            }
        }

        void ExecutionManagement::fillStates(
            const std::string &configFilepath,
            std::set<std::pair<std::string, std::string>> &functionGroupStates,
            std::map<std::string, std::string> &initialStates)
        {
            const arxml::ArxmlReader cArxmlReader(configFilepath);

            const arxml::ArxmlNodeRange cFunctionGroupNodes{
                cArxmlReader.GetNodes(
                    {"AUTOSAR", "AR-PACKAGES", "AR-PACKAGE", "ELEMENTS", "FUNCTION-GROUPS"})};

            for (const auto cFunctionGroupNode : cFunctionGroupNodes)
            {
                const std::string cShortName{cFunctionGroupNode.GetShortName()};
                const std::string cNodeContent{cFunctionGroupNode.GetContent()};

                fillInitialStates(
                    cShortName, cNodeContent, initialStates);
                fillFunctionGroupStates(
                    cShortName, cNodeContent, functionGroupStates);
            }
        }

        void ExecutionManagement::onStateChange(
            const std::map<std::string, std::string> &arguments)
        {
            const std::string cStartUpState{"StartUp"};

            std::string _currentState;
            if (mStateServer->TryGetState(cMachineFunctionGroup, _currentState) &&
                _currentState == cStartUpState)
            {
                mPlatformHealthManager.Initialize(arguments);
                mDiagnosticManager.Initialize(arguments);
                mExtendedVehicle.Initialize(arguments);
            }
        }

        int ExecutionManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            ara::log::LogStream _logStream;

            try
            {
                const std::string cConfigFilepath{
                    arguments.at(helper::ArgumentConfiguration::cConfigArgument)};

                const helper::RpcConfiguration cRpcConfiguration{
                    getRpcConfiguration(cConfigFilepath)};
                ara::com::someip::rpc::SocketRpcServer _rpcServer(
                    Poller,
                    cRpcConfiguration.ipAddress,
                    cRpcConfiguration.portNumber,
                    cRpcConfiguration.protocolVersion);

                ara::exec::ExecutionServer _executionServer(&_rpcServer);

                std::set<std::pair<std::string, std::string>> _functionGroupStates;
                std::map<std::string, std::string> _initialState;
                fillStates(cConfigFilepath, _functionGroupStates, _initialState);
                mStateServer =
                    new ara::exec::StateServer(&_rpcServer,
                                               std::move(_functionGroupStates),
                                               std::move(_initialState));

                auto _onStateChangeCallback{
                    std::bind(&ExecutionManagement::onStateChange, this, arguments)};
                mStateServer->SetNotifier(
                    cMachineFunctionGroup, _onStateChangeCallback);

                _logStream << "Execution management has been initialized.";
                Log(cLogLevel, _logStream);

                mStateManagement.Initialize(arguments);

                bool _running{true};

                while (!cancellationToken->load() && _running)
                {
                    _running = WaitForActivation();
                }

                int _evTerminationResult{mExtendedVehicle.Terminate()};
                int _dmTerminationResult{mDiagnosticManager.Terminate()};
                int _phmTerminationResult{mPlatformHealthManager.Terminate()};
                int _smTerminationResult{mStateManagement.Terminate()};

                _logStream.Flush();
                _logStream << "Execution management has been terminated.";
                Log(cLogLevel, _logStream);

                int _result{
                    _dmTerminationResult +
                    _evTerminationResult +
                    _phmTerminationResult +
                    _smTerminationResult};

                return _result;
            }
            catch (const std::runtime_error &ex)
            {
                _logStream.Flush();
                _logStream << ex.what();
                Log(cErrorLevel, _logStream);

                return cUnsuccessfulExitCode;
            }
        }

        ExecutionManagement::~ExecutionManagement()
        {
            mExtendedVehicle.Terminate();
            mDiagnosticManager.Terminate();
            mPlatformHealthManager.Terminate();
            mStateManagement.Terminate();

            if (mStateServer)
                delete mStateServer;
        }
    }
}