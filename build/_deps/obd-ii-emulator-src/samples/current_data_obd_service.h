#ifndef CURRENT_DATA_OBD_SERVICE_H
#define CURRENT_DATA_OBD_SERVICE_H

#include "../include/obdemulator/obd_service.h"

namespace ObdEmulator
{
    class CurrentDataObdService : public ObdService
    {
    private:
        static const uint8_t cService{0x01};

    public:
        // Show current data

        CurrentDataObdService() noexcept;

        bool TryGetResponse(
            const std::vector<uint8_t> &pid, std::vector<uint8_t> &response) override;
    };
}

#endif