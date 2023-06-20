#ifndef FIFO_CHECKPOINT_COMMUNICATOR
#define FIFO_CHECKPOINT_COMMUNICATOR

#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/fifo_sender.h>
#include <asyncbsdsocket/fifo_receiver.h>
#include "../../ara/phm/checkpoint_communicator.h"
#include "../../ara/com/helper/concurrent_queue.h"
#include "../../ara/com/e2e/profile11.h"

namespace application
{
    namespace helper
    {
        /// @brief Supervision checkpoint communicator over the FIFO medium
        class FifoCheckpointCommunicator : public ara::phm::CheckpointCommunicator
        {
        private:
            static const std::size_t cE2eHeaderSize;
            static const std::size_t cBufferSize;

            AsyncBsdSocketLib::Poller *const mPoller;
            AsyncBsdSocketLib::FifoSender mClient;
            AsyncBsdSocketLib::FifoReceiver mServer;
            ara::com::e2e::Profile11 mProfile11;
            ara::com::helper::ConcurrentQueue<std::vector<uint8_t>> mSendQueue;

            void onCheckpointSend();
            void onCheckpointReceive();

        public:
            /// @brief Constructor
            /// @param poller Master poller
            /// @param fifoPath FIFO file path
            /// @throws std::runtime_error Thrown when the FIFO communication setup failed
            FifoCheckpointCommunicator(
                AsyncBsdSocketLib::Poller *poller,
                std::string fifoPath);

            ~FifoCheckpointCommunicator() override;

            bool TrySend(uint32_t checkpoint) override;
        };
    }
}

#endif