#ifndef CONTROLLER_CONFIG_H
#define CONTROLLER_CONFIG_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief DoIP controller configuration data structure
    /// @see DoipController
    struct ControllerConfig
    {
        /// @brief DoIP protocol version
        uint8_t protocolVersion;

        /// @brief Maximum DoIP request payload length
        uint32_t doipMaxRequestBytes;

        /// @brief Delay in seconds before starting the initial vehicle announcement
        std::chrono::seconds doIPInitialVehicleAnnouncementTime;

        /// @brief Number of initial vehicle annoucement interations
        uint8_t doIPVehicleAnnouncementCount;

        /// @brief Interval in seconds between each initial vehicle announcement
        std::chrono::seconds doIPVehicleAnnouncementInterval;
    };
}

#endif