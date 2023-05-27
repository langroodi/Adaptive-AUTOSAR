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

    void ObdService::SetCallback(CallbackType &&callback)
    {
        Callback =
            std::bind(
                std::move(callback),
                std::placeholders::_1,
                std::placeholders::_2,
                mService);
    }

    void ObdService::ResetCallback() noexcept
    {
        Callback = nullptr;
    }

    ObdService::~ObdService() noexcept
    {
        ResetCallback();
    }
}