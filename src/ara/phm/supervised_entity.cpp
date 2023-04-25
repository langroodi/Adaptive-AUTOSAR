#include "./supervised_entity.h"

#include <utility>

namespace ara
{
    namespace phm
    {
        SupervisedEntity::SupervisedEntity(
            const core::InstanceSpecifier &instance,
            CheckpointCommunicator *communicator) : mInstance{instance},
                                                    mCommunicator{communicator}
        {
        }

        SupervisedEntity::SupervisedEntity(
            SupervisedEntity &&se) noexcept : mInstance{std::move(se.mInstance)},
                                              mCommunicator{se.mCommunicator}
        {
        }
    }
}