#include "../include/obdemulator/obd_service.h"

namespace ObdEmulator
{
    ObdService::ObdService(uint8_t service) noexcept : mService{service}
    {
    }

    uint8_t ObdService::GetService() const noexcept
    {
        return mService;
    }
}