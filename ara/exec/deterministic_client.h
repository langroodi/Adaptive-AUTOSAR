#include <stdint.h>

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
    }
}