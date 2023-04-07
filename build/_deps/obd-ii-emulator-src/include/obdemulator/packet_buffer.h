#ifndef PACKET_BUFFER_H
#define PACKET_BUFFER_H

#include <atomic>
#include <mutex>
#include <queue>
#include <vector>

namespace ObdEmulator
{
    /// @brief Thread-safe queue for buffering packet data bytes
    class PacketBuffer
    {
    private:
        std::mutex mMutex;
        std::queue<std::vector<uint8_t>> mQueue;
        std::atomic_size_t mSize;
        std::unique_lock<std::mutex> mLock;

    public:
        PacketBuffer();

        ~PacketBuffer() = default;

        /// @brief Indicate whether the buffer is empty or not
        /// @returns True if the buffer is empty, otherwise false
        inline bool Empty() const noexcept
        {
            return mSize == 0;
        }

        /// @brief Try to insert a packet to the buffer via moving
        /// @param[in] packet Packet bytes to be moved into the buffer
        /// @returns True if the packet is moved to the buffer successfully, otherwise false
        bool TryEnqueue(std::vector<uint8_t> &&packet);

        /// @brief Try to peek a packet from the buffer by removing it
        /// @param[out] packet Packet that is moved out from the buffer
        /// @returns True if the packet is dequeued successfully, otherwise false
        bool TryDequeue(std::vector<uint8_t> &packet);

        /// @brief Try to make the buffer empty
        /// @returns True if the buffer is cleared successfully; otherwise false
        bool TryClear();
    };
}

#endif