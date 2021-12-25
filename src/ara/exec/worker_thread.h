#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <atomic>
#include <stdint.h>
#include <random>

namespace ara
{
    namespace exec
    {
        /// @brief DeterministicClient random number generator
        class WorkerThread
        {
        private:
            static std::atomic_uint64_t mOffset;
            static thread_local std::default_random_engine mGenerator;

        public:
            /// @brief Offset step that WorkerThread takes to generate the next unique random number
            static const uint64_t cOffsetStep = 6;

            WorkerThread() noexcept = default;
            virtual ~WorkerThread() noexcept = default;

            /// @brief Generate a unique random number for each container
            /// @returns Random integer number
            uint64_t GetRandom() noexcept;
        };
    }
}

#endif