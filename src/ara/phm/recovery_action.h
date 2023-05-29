#ifndef RECOVERY_ACTION_H
#define RECOVERY_ACTION_H

#include "../core/result.h"
#include "../core/instance_specifier.h"
#include "../exec/execution_error_event.h"

namespace ara
{
    namespace phm
    {
        /// @brief Supervision algorithm type
        enum class TypeOfSupervision : uint32_t
        {
            AliveSupervision = 0,    ///!< Supervision by measuring the time interval of a single checkpoint
            DeadlineSupervision = 1, ///!< Supervision by measuring the time difference between two checkpoints
            LogicalSupervision = 2   ///!< Supervision by controlling the occurrence sequence of several checkpoints
        };

        /// @brief An abstract class to wrap the action for recovering a supervised entity
        /// @note The class is not fully aligned with the ARA standard.
        /// @see SupervisedEntity
        class RecoveryAction
        {
        private:
            const core::InstanceSpecifier &mInstance;
            bool mOffered;

        protected:
            /// @brief Constructor
            /// @param instance Adaptive application instance that owns the entity
            explicit RecoveryAction(const core::InstanceSpecifier &instance);

            /// @brief Indicate whether the action has been already offered or not
            /// @returns True if the action has been already offered; otherwise false
            bool IsOffered() const noexcept;

        public:
            RecoveryAction(RecoveryAction &&ra) noexcept;
            RecoveryAction(const RecoveryAction &ra) = delete;
            RecoveryAction &operator=(const RecoveryAction &ra) = delete;

            virtual ~RecoveryAction() noexcept = default;

            /// @brief Perform the action to handle the recovery
            /// @param executionError Occurred error while supervising
            /// @param supervision Failed supervision algorithm type
            /// @note The action should be offered beforehand to enable the invocation.
            /// @see Offer
            virtual void RecoveryHandler(
                const exec::ExecutionErrorEvent &executionError,
                TypeOfSupervision supervision) = 0;

            /// @brief Start offering the recovery action
            /// @return Empty result in case of successful offering
            /// @remark It is safe to call the function, if the action has been already offered.
            core::Result<void> Offer();

            /// @brief Stop offering the recovery action
            /// @remark It is safe to call the function, if the action offer has been already stopped.
            void StopOffer() noexcept;
        };
    }
}

#endif