#ifndef MOCKED_CHECKPOINT_COMMUNICATOR_H
#define MOCKED_CHECKPOINT_COMMUNICATOR_H

#include "../../../src/ara/phm/checkpoint_communicator.h"

namespace ara
{
    namespace phm
    {
        class MockedCheckpointCommunicator : public CheckpointCommunicator
        {
        public:
            bool TrySend(uint32_t checkpoint) override
            {
                if (Callback)
                {
                    Callback(checkpoint);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };
    }
}

#endif