#ifndef LOG_RECOVERY_ACTION_H
#define LOG_RECOVERY_ACTION_H

#include "../../ara/phm/recovery_action.h"
#include "../../ara/log/logging_framework.h"

namespace application
{
    namespace helper
    {
        /// @brief A class to provide logging as the recovery action for a failed supervised entity
        class LogRecoveryAction : public ara::phm::RecoveryAction
        {
        private:
            static const ara::core::InstanceSpecifier cInstance;
            static const ara::log::LogMode cLogMode;
            static const std::string cContextId;
            static const std::string cContextDescription;
            static const ara::log::LogLevel cErrorLevel;

            ara::log::LoggingFramework *mLoggingFramework;
            ara::log::Logger mLogger;

        public:
            LogRecoveryAction();
            ~LogRecoveryAction() override;

            void RecoveryHandler(
                const ara::exec::ExecutionErrorEvent &executionError,
                ara::phm::TypeOfSupervision supervision) override;
        };
    }
}

#endif