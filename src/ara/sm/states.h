#ifndef STATES_H
#define STATES_H

#include <stdint.h>

namespace ara
{
    /// @brief Adaptive AUTOSAR State Management
    namespace sm
    {
        /// @brief Function group state
        enum class FunctionGroupStates : uint8_t
        {
            kOff = 0,       ///< State Off
            kRunning = 1,   ///< State Running
            kUpdate = 2,    ///< State Updating
            kVerify = 3     ///< State Update Verifying
        };
    }
}

#endif