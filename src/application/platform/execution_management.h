#ifndef EXECUTION_MANAGEMENT_H
#define EXECUTION_MANAGEMENT_H

#include <set>
#include "./state_management.h"

/// @brief AUTOSAR application namespace
namespace application
{
    /// @brief AUTOSAR platform application namespace
    namespace platform
    {
        /// @brief Execution managment modelled process
        class ExecutionManagement : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;
            static const ara::log::LogMode cLogMode;
            static const std::string cContextId;
            static const std::string cContextDescription;
            static const ara::log::LogLevel cLogLevel;
            static const ara::log::LogLevel cErrorLevel;

            StateManagement mStateManagement;
            ara::exec::DeterministicClient mDeterministicClient;
            ara::log::LoggingFramework *mLoggingFramework;
            ara::log::Logger mLogger;
            AsyncBsdSocketLib::Poller mPoller;

            static helper::RpcConfiguration getRpcConfiguration(
                const std::string &configFilepath);

            static void fillFunctionGroupStates(
                std::string functionGroupShortName,
                const std::string &functionGroupContent,
                std::set<std::pair<std::string, std::string>> &functionGroupStates);

            static void fillInitialStates(
                std::string functionGroupShortName,
                const std::string &functionGroupContent,
                std::map<std::string, std::string> &initialStates);

            static void fillStates(
                const std::string &configFilepath,
                std::set<std::pair<std::string, std::string>> &functionGroupStates,
                std::map<std::string, std::string> &initialStates);

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            ExecutionManagement();
            ~ExecutionManagement() override;
        };
    }
}

#endif