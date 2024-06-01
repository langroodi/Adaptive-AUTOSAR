#ifndef POWER_MODE_TYPE_H
#define POWER_MODE_TYPE_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief DoIP node power mode type for a reliable communication with a tester
    enum class PowerModeType : uint8_t
    {
        NotReady = 0x00,    ///!< Not-ready power state
        Ready = 0x01,       ///!< Ready power state
        NotSupported = 0x02 ///!< Unsupported power mode request
    };
}

#endif