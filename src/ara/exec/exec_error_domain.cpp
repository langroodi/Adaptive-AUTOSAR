#include "./exec_error_domain.h"

namespace ara
{
    namespace exec
    {
        const core::ErrorDomain::IdType ExecErrorDomain::cDomainId;

        ExecErrorDomain::ExecErrorDomain() noexcept : ErrorDomain{cDomainId}
        {
        }

        const char *ExecErrorDomain::Name() const noexcept
        {
            return "Execution Management error domain";
        }

        const char *ExecErrorDomain::Message(
            core::ErrorDomain::CodeType errorCode) const noexcept
        {
            ExecErrc _execErrc = static_cast<ExecErrc>(errorCode);

            switch (_execErrc)
            {
            case ExecErrc::kGeneralError:
                return "Unspecified error occurrence";
            case ExecErrc::kInvalidArguments:
                return "Invalid argument while setting the state";
            case ExecErrc::kCommunicationError:
                return "SOME/IP or FIFO communication error occurrence";
            case ExecErrc::kMetaModelError:
                return "Invalid ARXML meta-model";
            case ExecErrc::kCancelled:
                return "Cancelling of the on-going state transition due to a new state trainsition request";
            case ExecErrc::kFailed:
                return "Impossible operation to be performed";
            case ExecErrc::kFailedUnexpectedTerminationOnExit:
                return "Previous function group termination while state transition";
            case ExecErrc::kFailedUnexpectedTerminationOnEnter:
                return "Target function group termination while state transition";
            case ExecErrc::kInvalidTransition:
                return "Invalid state transition request";
            case ExecErrc::kAlreadyInState:
                return "Invalid state transition due to request the current state";
            case ExecErrc::kInTransitionToSameState:
                return "Transition failing due to equality to the on-going state transition";
            case ExecErrc::kNoTimeStamp:
                return "Unavailable deterministic timestamp information";
            case ExecErrc::kCycleOverrun:
                return "Exceeding theoritical cycle time";

            default:
                return "Unsupported error code";
            }
        }
    }
}