#include <utility>
#include "./state_management.h"
#include "../../arxml/arxml_reader.h"

namespace application
{
    namespace platform
    {
        const std::string StateManagement::cAppId{"StateManagement"};
        const ara::log::LogMode StateManagement::cLogMode{ara::log::LogMode::kConsole};
        const std::string StateManagement::cContextId{"Lifetime"};
        const std::string StateManagement::cContextDescription{"Application lifetime logs"};
        const ara::log::LogLevel StateManagement::cLogLevel{ara::log::LogLevel::kInfo};

        StateManagement::StateManagement() : mLoggingFramework{ara::log::LoggingFramework::Create(cAppId, cLogMode)},
                                             mLogger{mLoggingFramework->CreateLogger(cContextId, cContextDescription, cLogLevel)}
        {
        }

        void StateManagement::configureStates(
            const ara::exec::FunctionGroup &functionGroup,
            std::string &&functionGroupContent)
        {
            arxml::ArxmlReader _arxmlReader(
                functionGroupContent.c_str(), functionGroupContent.length());

            arxml::ArxmlNodeRange _functionGroupStateNodes{
                _arxmlReader.GetNodes(
                    {"FUNCTION-GROUP", "MODE-DECLARATION-GROUP", "MODE-DECLARATIONS"})};

            for (auto functionGroupStateNode : _functionGroupStateNodes)
            {
                std::string _shortName{functionGroupStateNode.GetShortName()};

                mFunctionGroupStates.push_back(
                    std::move(
                        ara::exec::FunctionGroupState::Create(
                            functionGroup, _shortName).Value()));

                ara::log::LogStream _logStream;
                std::string _functionGroupInstance{
                    functionGroup.GetInstance().ToString()};
                _logStream << "State: " << _shortName << " of function group: " << _functionGroupInstance << " is configured.";
                mLoggingFramework->Log(mLogger, cLogLevel, _logStream);
            }
        }

        void StateManagement::configureFunctionGroups(const std::string &configFilepath)
        {
            arxml::ArxmlReader _arxmlReader(configFilepath);

            arxml::ArxmlNodeRange _functionGroupNodes{
                _arxmlReader.GetNodes(
                    {"AUTOSAR", "AR-PACKAGES", "AR-PACKAGE", "ELEMENTS", "FUNCTION-GROUPS"})};

            for (auto functionGroupNode : _functionGroupNodes)
            {
                std::string _shortName{functionGroupNode.GetShortName()};

                mFunctionGroups.push_back(
                    std::move(ara::exec::FunctionGroup::Create(_shortName).Value()));

                ara::log::LogStream _logStream;
                _logStream << "Function group: " << _shortName << " is configured.";
                mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

                std::string _nodeContent{functionGroupNode.GetContent()};
                configureStates(mFunctionGroups.back(), std::move(_nodeContent));
            }
        }

        int StateManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const int cSuccessfulExitCode{0};
            const std::string cConfigArgument{"config"};

            ara::log::LogStream _logStream;

            std::string _configFilepath{arguments.at(cConfigArgument)};
            configureFunctionGroups(_configFilepath);

            _logStream << "State management has been initialized.";
            mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

            ara::exec::ActivationReturnType _activationReturn{
                ara::exec::ActivationReturnType::kInit};

            while (!cancellationToken->load() &&
                   _activationReturn != ara::exec::ActivationReturnType::kTerminate)
            {
                auto _activationReturnResult{mDeterministicClient.WaitForActivation()};
                _activationReturn = _activationReturnResult.Value();

                ///@todo State management logic here
            }

            _logStream.Flush();
            _logStream << "State management has been terminated.";
            mLoggingFramework->Log(mLogger, cLogLevel, _logStream);

            return cSuccessfulExitCode;
        }

        StateManagement::~StateManagement()
        {
            delete mLoggingFramework;
        }
    }
}