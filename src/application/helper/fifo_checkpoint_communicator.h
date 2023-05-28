#ifndef FIFO_CHECKPOINT_COMMUNICATOR
#define FIFO_CHECKPOINT_COMMUNICATOR

#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/fifo_sender.h>
#include <asyncbsdsocket/fifo_receiver.h>
#include "../../ara/phm/checkpoint_communicator.h"
#include "../../ara/com/helper/concurrent_queue.h"
#include "../../ara/com/helper/payload_helper.h"

namespace application
{
    namespace helper
    {
        /// @brief Supervision checkpoint communicator over the FIFO medium
        class FifoCheckpointCommunicator : public ara::phm::CheckpointCommunicator
        {
        private:
            static const std::string cFifoPath;
            static const std::size_t cBufferSize;

            AsyncBsdSocketLib::Poller *mPoller;
            AsyncBsdSocketLib::FifoSender mClient;
            AsyncBsdSocketLib::FifoReceiver mServer;
            ara::com::helper::ConcurrentQueue<std::vector<uint8_t>> mSendQueue;

            void onCheckpointSend();
            void onCheckpointReceive();

        public:
            /// @brief Constructor
            /// @param poller Master poller
            /// @throws std::runtime_error Thrown when the FIFO communication setup failed
            explicit FifoCheckpointCommunicator(
                AsyncBsdSocketLib::Poller *poller);

            ~FifoCheckpointCommunicator() override;

            bool TrySend(uint32_t checkpoint) override;
        };
    }
}

#endif