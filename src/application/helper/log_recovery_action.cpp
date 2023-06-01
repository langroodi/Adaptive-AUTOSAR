#include "./log_recovery_action.h"

namespace application
{
    namespace helper
    {
        const ara::core::InstanceSpecifier LogRecoveryAction::cInstance{"LogRecoveryAction"};
        const ara::log::LogMode LogRecoveryAction::cLogMode{ara::log::LogMode::kConsole};
        const std::string LogRecoveryAction::cContextId{"RecoveryAction"};
        const std::string LogRecoveryAction::cContextDescription{"Recovery action logs"};
        const ara::log::LogLevel LogRecoveryAction::cErrorLevel{ara::log::LogLevel::kError};

        LogRecoveryAction::LogRecoveryAction() : ara::phm::RecoveryAction(cInstance),
                                                 mLoggingFramework{ara::log::LoggingFramework::Create(cInstance.ToString(), cLogMode)},
                                                 mLogger{mLoggingFramework->CreateLogger(cContextId, cContextDescription, cErrorLevel)}
        {
        }

        void LogRecoveryAction::RecoveryHandler(
            const ara::exec::ExecutionErrorEvent &executionError,
            ara::phm::TypeOfSupervision supervision)
        {
            const ara::exec::ExecutionError cExtendedVehicleExpiration{0};

            if (IsOffered() &&
                executionError.executionError == cExtendedVehicleExpiration)
            {
                ara::log::LogStream _logStream;

                switch (supervision)
                {
                case ara::phm::TypeOfSupervision::AliveSupervision:
                    _logStream << "Alive supervision";
                    break;

                case ara::phm::TypeOfSupervision::DeadlineSupervision:
                    _logStream << "Deadline supervision";
                    break;

                default:
                    // Unsupported supervision type
                    return;
                }

                _logStream << " is expired on "
                           << executionError.functionGroup->GetInstance().ToString();
                
                mLoggingFramework->Log(mLogger, cErrorLevel, _logStream);
            }
        }

        LogRecoveryAction::~LogRecoveryAction()
        {
            delete mLoggingFramework;
        }
    }
}