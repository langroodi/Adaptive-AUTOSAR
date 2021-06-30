#include <stdint.h>
#include <vector>
#include <chrono>

namespace ara
{
    namespace exec
    {
        enum class ActivationReturnType : uint8_t
        {
            kRegisterService = 0,
            kServiceDiscovery = 1,
            kInit = 2,
            kRun = 3,
            kTerminate = 4
        };

        enum class ActivationTimeStampReturnType : uint8_t
        {
            kSuccess = 0,
            kNotAvailable = 1
        };

        class DeterministicClient
        {
        public:
            using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;
            DeterministicClient() noexcept;
            ~DeterministicClient();
            ActivationReturnType WaitForNextActivation() const noexcept;
            //void RunWorkerPool(Worker &runnableObj, std::vector &container) const noexcept;
            uint64_t GetRandom() const noexcept;
            ActivationTimeStampReturnType GetActivationTime(TimeStamp timeStamp) const noexcept;
            ActivationTimeStampReturnType GetNextActivationTime(TimeStamp timeStamp) const noexcept;
        };
    }
}