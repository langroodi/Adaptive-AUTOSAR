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

    protected:
        /// @brief Callback to be invoked when data received
        CallbackType Callback;

    public:
        CommunicationLayer() noexcept;
        virtual ~CommunicationLayer() noexcept = default;

        /// @brief Try to start the communication
        /// @param configuration Configuration packet to sent to the endpoint right after the communication start
        /// @returns True if the communication start was successful, otherwise false
        virtual bool TryStart(std::vector<uint8_t> &&configuration) = 0;

        /// @brief Set a data received callaback
        /// @param callback Callback to be invoked when a data is received
        void SetCallback(CallbackType &&callback);

        /// @brief Reset the data received callback if it has been already set
        /// @see SetCallback
        void ResetCallback() noexcept;

        /// @brief Try to stop the communication
        /// @returns True if the communication stop was successful, otherwise false
        /// @note As the method side effect, it may clear the send and receive buffer
        virtual bool TryStop() = 0;
    };
}

#endif