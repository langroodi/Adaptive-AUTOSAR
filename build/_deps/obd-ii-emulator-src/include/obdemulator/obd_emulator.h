#ifndef OBD_EMULATOR_H
#define OBD_EMULATOR_H

#include <map>
#include "./communication_layer.h"
#include "./can_driver.h"
#include "./obd_service.h"

namespace ObdEmulator
{
    /// @brief A class that routes OBD queries to their corresponding services
    class ObdEmulator
    {
    private:
        CommunicationLayer *mCommunicationLayer;
        const CanDriver *mCanDriver;
        std::map<uint8_t, ObdService *> mObdServices;

        bool tryParseQuery(
            std::vector<uint8_t> &&query,
            std::vector<uint8_t> &pid,
            uint8_t &queriedService) const;

        void generateResponse(
            const std::vector<uint8_t> &pid,
            std::vector<uint8_t> &&serviceResponseData,
            uint8_t queriedService,
            std::vector<uint8_t> &response) const;

        bool processQuery(
            std::vector<uint8_t> &&query,
            std::vector<uint8_t> &response) const;

        void sendResponseAsync(
            const std::vector<uint8_t> &pid,
            std::vector<uint8_t> &&serviceResponseData,
            uint8_t queriedService);

        void processQueryAsync(std::vector<uint8_t> &&query);

    public:
        /// @brief Constructor
        /// @param communicationLayer CAN communication medium abstraction layer
        /// @param canDriver CAN USB to serial communication driver
        /// @param obdServices Supported OBD-II services during the emulation
        ObdEmulator(
            CommunicationLayer *communicationLayer,
            const CanDriver *canDriver,
            std::initializer_list<ObdService *> obdServices);
        ~ObdEmulator();

        /// @brief Try to start the emulation
        /// @returns True if the emulation start was successful, otherwise false
        bool TryStart();

        /// @brief Try to start the emulation for asynchrous data handling
        /// @returns True if the emulation start was successful, otherwise false
        bool TryStartAsync();

        /// @brief Try to stop the emulation
        /// @returns True if the emulation stop was successful, otherwise false
        bool TryStop();
    };
}

#endif