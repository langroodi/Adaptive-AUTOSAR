#ifndef EXEC_ERROR_DOMAIN_H
#define EXEC_ERROR_DOMAIN_H

#include "../core/error_domain.h"

namespace ara
{
    /// @brief Adaptive AUTOSAR Execution Management 
    namespace exec
    {
        /// @brief Execution Management error codes
        enum class ExecErrc : ara::core::ErrorDomain::CodeType
        {
            kGeneralError = 1,                          ///< Unspecified error occurrence
            kInvalidArguments = 2,                      ///< Invalid argument while setting the state
            kCommunicationError = 3,                    ///< SOME/IP or FIFO communication error occurrence
            kMetaModelError = 4,                        ///< Invalid ARXML meta-model
            kCancelled = 5,                             ///< Cancelling of the on-going state transition due to a new state trainsition request
            kFailed = 6,                                ///< Impossible operation to be performed
            kFailedUnexpectedTerminationOnExit = 7,     ///< Previous function group termination while state transition
            kFailedUnexpectedTerminationOnEnter = 8,    ///< Target function group termination while state transition
            kInvalidTransition = 9,                     ///< Invalid state transition request
            kAlreadyInState = 10,                       ///< Invalid state transition due to request the current state
            kInTransitionToSameState = 11,              ///< Transition failing due to equality to the on-going state transition
            kNoTimeStamp = 12,                          ///< Unavailable deterministic timestamp information
            kCycleOverrun = 13                          ///< Exceeding theoritical cycle time
        };                  

        /// @brief Execution Management ErrorDomain
        class ExecErrorDomain final : public core::ErrorDomain
        {
        private:
            static const core::ErrorDomain::IdType cDomainId{0x8000000000000202};

        public:
            ExecErrorDomain() noexcept;

            const char *Name() const noexcept override;

            const char *Message(
                core::ErrorDomain::CodeType errorCode) const noexcept override;
        };
    }
}

#endif