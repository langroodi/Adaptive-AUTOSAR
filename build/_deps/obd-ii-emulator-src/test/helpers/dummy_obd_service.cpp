#include <cstddef>
#include "./dummy_obd_service.h"

namespace ObdEmulator
{
    namespace Helpers
    {
        const uint8_t DummyObdService::cService;
        const uint8_t DummyObdService::cVehicleSpeedPid;
        const uint8_t DummyObdService::cVehicleSpeed;

        DummyObdService::DummyObdService() noexcept : ObdService(cService)
        {
        }

        bool DummyObdService::TryGetResponse(
            const std::vector<uint8_t> &pid,
            std::vector<uint8_t> &response) const
        {
            const size_t cPidIndex{0};
            uint8_t _queriedPid{pid.at(cPidIndex)};

            if (_queriedPid == cVehicleSpeedPid)
            {
                response = {cVehicleSpeed};
                return true;
            }
            else
            {
                return false;
            }
        }

        bool DummyObdService::TryGetResponseAsync(const std::vector<uint8_t> &pid)
        {
            const size_t cPidIndex{0};
            uint8_t _queriedPid{pid.at(cPidIndex)};

            if (_queriedPid == cVehicleSpeedPid)
            {
                std::vector<uint8_t> _response{cVehicleSpeed};
                Callback(pid, std::move(_response), GetService());
                
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}