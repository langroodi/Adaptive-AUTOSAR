#include "../include/obdemulator/communication_layer.h"

namespace ObdEmulator
{
    CommunicationLayer::CommunicationLayer() noexcept : Callback{nullptr},
                                                        AsyncCallback{nullptr}
    {
    }

    void CommunicationLayer::SetCallback(CallbackType &&callback)
    {
        Callback = std::move(callback);
        AsyncCallback = nullptr;
    }

    void CommunicationLayer::SetCallback(AsyncCallbackType &&asyncCallback)
    {
        AsyncCallback = std::move(asyncCallback);
        Callback = nullptr;
    }

    void CommunicationLayer::ResetCallback() noexcept
    {
        Callback = nullptr;
        AsyncCallback = nullptr;
    }
}