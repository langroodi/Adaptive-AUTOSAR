#ifndef DETERMINISTIC_CLIENT_H
#define DETERMINISTIC_CLIENT_H

#include <stdint.h>
#include <vector>
#include <chrono>
#include "../core/result.h"
#include "./worker_runnable.h"

namespace ara
{
    namespace exec
    {
        /// @brief Application deterministic behaviour lifecycle action
        enum class ActivationReturnType : uint8_t
        {
            kRegisterService = 0,   ///!< Application shall register its service server
            kServiceDiscovery = 1,  ///!< Application shall discover its service server
            kInit = 2,              ///!< Application shall intialize itself
            kRun = 3,               ///!< Application shall perform its normal operation
            kTerminate = 4          ///!< Application shall terminate itself
        };

        /// @brief Class that utilizes a client to have deterministic behaviour
        class DeterministicClient
        {
        public:
            /// @brief Steady clock time point for deterministic cycles
            using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;

            DeterministicClient() noexcept;
            ~DeterministicClient() noexcept;

            /// @brief Blocks the caller till reaching the next activation time
            /// @returns Value that controls the caller's internal lifecylce
            core::Result<ActivationReturnType> WaitForActivation() noexcept;

            /// @brief Run a deterministic worker pool to process a container via a runnable object
            /// @details It uses the pool to iterate over each container element and call WorkerRunnable::Run of the runnable object for each of them.
            /// @tparam ValueType Container element type
            /// @tparam Container Container type which supports standard iterator operators (e.g., begin and end)
            /// @param runnableObj Object to process container elements
            /// @param container Wrapper around the elements that the WorkerRunnable should process
            /// @returns Void Result if running the pool was successful, otherwise Result which contains the occurred error
            template <typename ValueType, typename Container>
            //static_assert(std::is_same<ValueType, typename Container::value_type>::value, "Container value type mismatch!");
            ara::core::Result<void> RunWorkerPool(
                WorkerRunnable<ValueType> &runnableObj,
                Container &container) noexcept;
            
            /// @brief Get a deterministic random number
            /// @returns Identical random number till the next activation cycle
            uint64_t GetRandom() noexcept;

            /// @brief Set a seed number to the deterministic random number generator
            /// @param seed Random number seed
            void SetRandomSeed (uint64_t seed) noexcept;

            /// @brief Get the last activation time
            /// @returns Last time that WaitForActivation returns ActivationReturnType::kRun
            core::Result<TimeStamp> GetActivationTime() noexcept;

            /// @brief Get the next comming activation time
            /// @returns Time that WaitForActivation returns ActivationReturnType::kRun at a new cycle
            core::Result<TimeStamp> GetNextActivationTime() noexcept;
        };
    }
}

#endif