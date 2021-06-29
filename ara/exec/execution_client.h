#include <stdint.h>

namespace ara
{
    namespace exec
    {
        enum class ExecutionState : uint8_t
        {
            kRunning = 0,
            kTerminating = 1
        };

        class ExecutionClient final
        {
        public:
            ExecutionClient() noexcept;
            ~ExecutionClient() noexcept;
            
        };
    }
}