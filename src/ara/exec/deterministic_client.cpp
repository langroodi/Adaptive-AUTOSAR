#include <thread>
#include "./deterministic_client.h"

namespace ara
{
    namespace exec
    {
        const uint64_t DeterministicClient::cCycleDelayMs;
        std::atomic_uint8_t DeterministicClient::mCounter;
        std::future<void> DeterministicClient::mFuture;
        bool DeterministicClient::mRunning;
        std::mutex DeterministicClient::mCycleMutex;
        std::condition_variable DeterministicClient::mCycleConditionVariable;
        std::default_random_engine DeterministicClient::mGenerator;
        std::uniform_int_distribution<uint64_t> DeterministicClient::mDistribution;
        helper::AtomicOptional<uint64_t> DeterministicClient::mSeed;
        uint64_t DeterministicClient::mRandomNumber;
        DeterministicClient::TimeStamp  DeterministicClient::mActivationTime;

        DeterministicClient::DeterministicClient() 
        {
            ++mCounter;

            // Check whether the constructing object is the first instance or not
            if (mCounter == 1)
            {
                mRunning = true;

                mFuture =
                    std::async(
                        std::launch::async, &DeterministicClient::activateCycle);
            }
        }

        void DeterministicClient::activateCycle()
        {
            const std::chrono::milliseconds cCycleDuration{cCycleDelayMs};

            while (mRunning)
            {
                // Apply the seed if it was requested
                if (mSeed.HasValue())
                {
                    mGenerator.seed(mSeed.Value());
                    mSeed.Reset();
                }

                mRandomNumber = mDistribution(mGenerator);
                mActivationTime = std::chrono::steady_clock::now();
                mCycleConditionVariable.notify_all();

                std::this_thread::sleep_for(cCycleDuration);
            }
        }

        core::Result<ActivationReturnType> DeterministicClient::WaitForActivation()
        {
            std::unique_lock<std::mutex> _lock(mCycleMutex);
            mCycleConditionVariable.wait(_lock);
            core::Result<ActivationReturnType> _result{ActivationReturnType::kRun};
            
            return _result;
        }

        uint64_t DeterministicClient::GetRandom() noexcept
        {
            return mRandomNumber;
        }

        void DeterministicClient::SetRandomSeed(uint64_t seed) noexcept
        {
            mSeed = seed;
        }

        core::Result<DeterministicClient::TimeStamp> DeterministicClient::GetActivationTime() noexcept
        {
            core::Result<TimeStamp> _result{mActivationTime};
            return _result;
        }

        core::Result<DeterministicClient::TimeStamp> DeterministicClient::GetNextActivationTime()
        {
            const std::chrono::milliseconds cCycleDuration{cCycleDelayMs};
            // Estimate the next activation time based on the cycle duration
            TimeStamp _nextActivationTime = mActivationTime + cCycleDuration;
            core::Result<TimeStamp> _result{_nextActivationTime};

            return _nextActivationTime;
        }

        DeterministicClient::~DeterministicClient()
        {
            // Check whether the desctructing object is the last instance or not
            if (mCounter == 1)
            {
                mRunning = false;

                // Wait for the cycling thread to exit gracefully
                mFuture.get();
            }

            --mCounter;
        }
    }
}