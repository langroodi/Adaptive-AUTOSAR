#include "./worker_thread.h"

namespace ara
{
    namespace exec
    {
        const uint64_t WorkerThread::cOffsetStep;
        std::atomic_uint64_t WorkerThread::mOffset;
        thread_local std::default_random_engine WorkerThread::mGenerator;

        uint64_t WorkerThread::GetRandom() noexcept
        {
            std::uniform_int_distribution<uint64_t> _distribution{0, cOffsetStep - 1};
            uint64_t _randomNumber = _distribution(mGenerator);
            uint64_t _result = mOffset + _randomNumber;
            mOffset += cOffsetStep;

            return _result;
        };
    }
}