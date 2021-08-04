#ifndef STATES_H
#define STATES_H

#include <stdint.h>

namespace ara
{
    namespace sm
    {
        enum class FunctionGroupStates : uint8_t
        {
            kOff = 0,
            kRunning = 1,
            kUpdate = 2,
            kVerify = 3
        };
    }
}

#endif