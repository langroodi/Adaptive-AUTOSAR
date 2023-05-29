#ifndef COMMUNICATION_LAYER_H
#define COMMUNICATION_LAYER_H

#include <functional>
#include <vector>
#include <stdint.h>

namespace ObdEmulator
{
    /// @brief Communication medium abstraction layer to send and receive data
    class CommunicationLayer
    {
    public:
        /// @brief Data received callback type
        /// @details The caller moves the received byte array to the callback and expects the handler to return a boolean that indiciates whether or not it fills the other byte array refenrece as the response.
        using CallbackType = std::function<bool(std::vector<uint8_t> &&, std::vector<uint8_t> &)>;

        /// @brief Data received callback type to handle the data asynchronously
        using AsyncCallbackType = std::function<void(std::vector<uint8_t> &&)>;

    protected:
        /// @brief Callback to be invoked when data received
        CallbackType Callback;

        /// @brief Callback to be invoked when data received asynch
        AsyncCallbackType AsyncCallback;

    public:
        CommunicationLayer() noexcept;
        virtual ~CommunicationLayer() noexcept = default;

        /// @brief Try to start the communication
        /// @param configuration Configuration packet to sent to the endpoint right after the communication start
        /// @returns True if the communication start was successful, otherwise false
        virtual bool TryStart(std::vector<uint8_t> &&configuration) = 0;

        /// @brief Try to buffer data for sending
        /// @param data Data to be buffered for sending
        /// @return True if the data is buffered successfully, otherwise false
        virtual bool TrySendAsync(std::vector<uint8_t> &&data) = 0;

        /// @brief Set a data received callback and reset the asynchronized callback
        /// @param callback Callback to be invoked when a data is received
        void SetCallback(CallbackType &&callback);

        /// @brief Set a data received asynchronous callback and reset the synchronized callback
        /// @param asyncCallback Callback to be invoked when a data is received for asynchronous data handling
        void SetCallback(AsyncCallbackType &&asyncCallback);

        /// @brief Reset both data received synchronized and asynchronized callbacks
        /// @see SetCallback(CallbackType)
        /// @see SetCallback(AsyncCallbackType)
        void ResetCallback() noexcept;

        /// @brief Try to stop the communication
        /// @returns True if the communication stop was successful, otherwise false
        /// @note As the method side effect, it may clear the send and receive buffer
        virtual bool TryStop() = 0;
    };
}

#endif