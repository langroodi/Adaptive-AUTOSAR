#include "./checkpoint_communicator.h"

namespace ara
{
    namespace phm
    {
        void CheckpointCommunicator::SetCallback(CheckpointReception &&callback)
        {
            Callback = std::move(callback);
        }

        void CheckpointCommunicator::ResetCallback() noexcept
        {
            Callback = nullptr;
        }

        CheckpointCommunicator::~CheckpointCommunicator() noexcept
        {
            ResetCallback();
        }
    }
}