#ifndef DUMMY_OBD_SERVICE_H
#define DUMMY_OBD_SERVICE_H

#include <obdemulator/obd_service.h>

namespace ObdEmulator
{
    namespace Helpers
    {
        class DummyObdService : public ObdService
        {
        public:
            static const uint8_t cService{0x01}; // Show current data
            static const uint8_t cVehicleSpeedPid{0x0d};
            static const uint8_t cVehicleSpeed{0x32}; // 50 km/h

            DummyObdService() noexcept;

            bool TryGetResponse(
                const std::vector<uint8_t> &pid, std::vector<uint8_t> &response) override;
        };
    }
}

#endif