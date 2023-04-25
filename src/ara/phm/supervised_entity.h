#ifndef SUPERVISED_ENTITY_H
#define SUPERVISED_ENTITY_H

#include <assert.h>
#include <type_traits>
#include "../core/instance_specifier.h"
#include "./checkpoint_communicator.h"

namespace ara
{
    namespace phm
    {
        /// @brief A class that collects and reports supervision checkpoints
        /// @note The class is not fully compatible with the ARA standard.
        class SupervisedEntity
        {
        private:
            const core::InstanceSpecifier &mInstance;
            CheckpointCommunicator *const mCommunicator;

        public:
            /// @brief Constructor
            /// @param instance Adaptive application instance that owns the entity
            /// @param communicator A communication medium for reporting the checkpoints
            SupervisedEntity(const core::InstanceSpecifier &instance,
                             CheckpointCommunicator *communicator);

            SupervisedEntity(SupervisedEntity &&se) noexcept;
            SupervisedEntity(const SupervisedEntity &se) = delete;
            SupervisedEntity &operator=(const SupervisedEntity &se) = delete;

            ~SupervisedEntity() noexcept = default;

            /// @brief Report a checkpoint to the PHM functional cluster
            /// @tparam EnumT Type of the checkpoint
            /// @param checkpointId ID of the checkpoint to be reported
            template <typename EnumT>
            void ReportCheckpoint(EnumT checkpointId)
            {
                constexpr const bool cIsSame{
                    std::is_same<std::underlying_type_t<EnumT>, uint32_t>::value};
                assert(cIsSame);

                auto _checkpoint{static_cast<uint32_t>(checkpointId)};
                mCommunicator->TrySend(_checkpoint);
            }
        };
    }
}

#endif