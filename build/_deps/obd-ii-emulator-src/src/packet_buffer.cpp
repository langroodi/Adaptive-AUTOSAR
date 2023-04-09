#include "../include/obdemulator/packet_buffer.h"

namespace ObdEmulator
{
    PacketBuffer::PacketBuffer() : mSize{0}
    {
    }

    bool PacketBuffer::TryEnqueue(std::vector<uint8_t> &&packet)
    {
        std::unique_lock<std::mutex> _lock(mMutex, std::defer_lock);
        if (_lock.try_lock())
        {
            mQueue.push(std::move(packet));
            ++mSize;
            _lock.unlock();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PacketBuffer::TryDequeue(std::vector<uint8_t> &packet)
    {
        std::unique_lock<std::mutex> _lock(mMutex, std::defer_lock);
        if (_lock.try_lock())
        {
            packet = std::move(mQueue.front());
            mQueue.pop();
            --mSize;
            _lock.unlock();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PacketBuffer::TryClear()
    {
        std::unique_lock<std::mutex> _lock(mMutex, std::defer_lock);
        std::queue<std::vector<uint8_t>> _emptyQueue;
        if (_lock.try_lock())
        {
            std::swap(mQueue, _emptyQueue);
            mSize = 0;
            _lock.unlock();
            return true;
        }
        else
        {
            return false;
        }
    }
}