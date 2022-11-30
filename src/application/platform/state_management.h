#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include "../../ara/exec/helper/modelled_process.h"
#include "../../ara/exec/deterministic_client.h"
#include "../../ara/exec/state_client.h"
#include "../../ara/log/logging_framework.h"

namespace application
{
    namespace platform
    {
        /// @brief State managment modelled process
        class StateManagement : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;
            static const ara::log::LogMode cLogMode;
            static const std::string cContextId;
            static const std::string cContextDescription;
            static const ara::log::LogLevel cLogLevel;

            ara::exec::DeterministicClient mDeterministicClient;
            ara::log::LoggingFramework *mLoggingFramework;
            ara::log::Logger mLogger;
            std::vector<ara::exec::FunctionGroup> mFunctionGroups;
            std::vector<ara::exec::FunctionGroupState> mFunctionGroupStates;

            void configureStates(
                const ara::exec::FunctionGroup &functionGroup,
                std::string &&functionGroupContent);
            void configureFunctionGroups(const std::string &configFilepath);

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            StateManagement();
            ~StateManagement() override;
        };
    }
}

#endif