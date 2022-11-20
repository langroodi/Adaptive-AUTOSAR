#ifndef EXECUTION_MANAGEMENT_H
#define EXECUTION_MANAGEMENT_H

#include "./state_management.h"

namespace application
{
    namespace platform
    {
        class ExecutionManagement : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;
            static const ara::log::LogMode cLogMode;
            static const std::string cContextId;
            static const std::string cContextDescription;
            static const ara::log::LogLevel cLogLevel;

            StateManagement mStateManagement;
            ara::exec::DeterministicClient mDeterministicClient;
            ara::log::LoggingFramework *mLoggingFramework;
            ara::log::Logger mLogger;

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