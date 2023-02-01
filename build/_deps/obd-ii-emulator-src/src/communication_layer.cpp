#include "../include/obdemulator/communication_layer.h"

namespace ObdEmulator
{
    CommunicationLayer::CommunicationLayer() noexcept : Callback{nullptr}
    {
    }
    
    void CommunicationLayer::SetCallback(CallbackType &&callback)
    {
        Callback = std::move(callback);
    }

    void CommunicationLayer::ResetCallback() noexcept
    {
        Callback = nullptr;
    }
}