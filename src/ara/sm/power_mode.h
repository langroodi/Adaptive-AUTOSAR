#ifndef POWER_MODE_H
#define POWER_MODE_H

namespace ara
{
    namespace sm
    {
        /// @brief Power mode request message
        enum class PowerModeMsg
        {
            kOn,        ///< Normal operation mode
            kOff,       ///< Preparation mode for shutdown
            kSuspend,   ///< Preparation mode for RAM and/or disk suspension
        };

        /// @brief Power mode response message
        enum class PowerModeRespMsg
        {
            kDone,          ///< Requested power mode reached successfully
            kFailed,        ///< Requested power mode reaching failed
            kBusy,          ///< Requested power mode ignored due to busy processing list
            kNotSupported   ///< Requested power mode not supported
        };
    }
}

#endif