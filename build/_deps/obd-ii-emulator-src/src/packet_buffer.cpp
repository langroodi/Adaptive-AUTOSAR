#include "../include/obdemulator/packet_buffer.h"

namespace ObdEmulator
{
    PacketBuffer::PacketBuffer() : mSize{0},
                                   mLock{mMutex, std::defer_lock}
    {
    }

    bool PacketBuffer::TryEnqueue(std::vector<uint8_t> &&packet)
    {
        if (mLock.try_lock())
        {
            mQueue.push(std::move(packet));
            ++mSize;
            mLock.unlock();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PacketBuffer::TryDequeue(std::vector<uint8_t> &packet)
    {
        if (mLock.try_lock())
        {
            packet = std::move(mQueue.front());
            mQueue.pop();
            --mSize;
            mLock.unlock();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PacketBuffer::TryClear()
    {
        std::queue<std::vector<uint8_t>> _emptyQueue;
        if (mLock.try_lock())
        {
            std::swap(mQueue, _emptyQueue);
            mSize = 0;
            mLock.unlock();
            return true;
        }
        else
        {
            return false;
        }
    }
}