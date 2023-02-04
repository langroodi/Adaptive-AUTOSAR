#ifndef DETERMINISTIC_CLIENT_H
#define DETERMINISTIC_CLIENT_H

#include <stdint.h>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "../core/result.h"
#include "./worker_runnable.h"
#include "./helper/atomic_optional.h"

namespace ara
{
    namespace exec
    {
        /// @brief Application deterministic behaviour lifecycle action
        enum class ActivationReturnType : uint8_t
        {
            kRegisterService = 0,  ///< Application shall register its service server
            kServiceDiscovery = 1, ///< Application shall discover its service server
            kInit = 2,             ///< Application shall intialize itself
            kRun = 3,              ///< Application shall perform its normal operation
            kTerminate = 4         ///< Application shall terminate itself
        };

        /// @brief Class that utilizes a client to have deterministic behaviour
        class DeterministicClient final
        {
        public:
            /// @brief Steady clock time point for deterministic cycles
            using TimeStamp = std::chrono::time_point<std::chrono::steady_clock>;

            /// @brief Theoretical cyclic delay in millisecond
            static const uint64_t cCycleDelayMs{10};

        private:
            static std::atomic_uint8_t mCounter;
            static std::future<void> mFuture;
            static bool mRunning;
            static std::mutex mCycleMutex;
            static std::condition_variable mCycleConditionVariable;
            static std::default_random_engine mGenerator;
            static std::uniform_int_distribution<uint64_t> mDistribution;
            static helper::AtomicOptional<uint64_t> mSeed;
            static uint64_t mRandomNumber;
            static TimeStamp mActivationTime;
            WorkerThread mWorkerThread;

            static void activateCycle();

        public:
            DeterministicClient();
            ~DeterministicClient();

            /// @brief Blocks the caller till reaching the next activation time
            /// @returns Value that controls the caller's internal lifecylce
            core::Result<ActivationReturnType> WaitForActivation();

            /// @brief Run a deterministic worker pool to process a container via a runnable object
            /// @details It uses the pool to iterate over each container element and call WorkerRunnable::Run of the runnable object for each of them.
            /// @tparam ValueType Container element type
            /// @tparam Container Container type which supports standard iterator operators (e.g., begin and end)
            /// @param runnableObj Object to process container elements
            /// @param container Wrapper around the elements that the WorkerRunnable should process
            /// @returns Void Result if running the pool was successful, otherwise Result which contains the occurred error
            template <typename ValueType, typename Container>
            ara::core::Result<void> RunWorkerPool(
                WorkerRunnable<ValueType> &runnableObj,
                Container &container) noexcept
            {
                static_assert(
                    std::is_same<ValueType, typename Container::value_type>::value,
                    "Container value type mismatch!");

                for (auto element : container)
                {
                    runnableObj.Run(element, mWorkerThread);
                }

                ara::core::Result<void> _result;
                return _result;
            }

            /// @brief Get a deterministic random number
            /// @returns Identical random number till the next activation cycle
            uint64_t GetRandom() noexcept;

            /// @brief Set a seed number to the deterministic random number generator
            /// @param seed Random number seed
            void SetRandomSeed(uint64_t seed) noexcept;

            /// @brief Get the last activation time
            /// @returns Last time that WaitForActivation returns ActivationReturnType::kRun
            core::Result<TimeStamp> GetActivationTime() noexcept;

            /// @brief Get the next comming activation time
            /// @returns Time that WaitForActivation returns ActivationReturnType::kRun at a new cycle
            core::Result<TimeStamp> GetNextActivationTime();
        };
    }
}

#endif