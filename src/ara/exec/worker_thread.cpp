#include "./worker_thread.h"

namespace ara
{
    namespace exec
    {
        const uint64_t WorkerThread::cOffsetStep;
        std::atomic_uint64_t WorkerThread::mOffset;

        WorkerThread::WorkerThread() : mDistribution{0, cOffsetStep - 1}
        {
        }

        uint64_t WorkerThread::GetRandom() noexcept
        {
            uint64_t _randomNumber = mDistribution(mGenerator);
            uint64_t _result = mOffset + _randomNumber;
            mOffset += cOffsetStep;

            return _result;
        };
    }
}