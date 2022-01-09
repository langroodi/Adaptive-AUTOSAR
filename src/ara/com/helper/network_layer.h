#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <map>
#include <functional>
#include <type_traits>

namespace ara
{
    namespace com
    {
        namespace helper
        {
            /// @brief Network communication abstraction layer
            /// @tparam T Message type
            template <typename T>
            class NetworkLayer
            {
                static_assert(
                    std::is_copy_assignable<T>::value,
                    "The template argument is not copy assignable.");

            private:
                std::map<void*, std::function<void(T)>> mReceiverCallbacks;

            protected:
                /// @brief Fire all the set receiver callaback
                /// @param message Received message
                void FireReceiverCallbacks(T &&message)
                {
                    // Copy the received message
                    T _receivedMessage = message;

                    for (auto objectCallbackPair : mReceiverCallbacks)
                    {
                        std::function<void(T)> _callback = objectCallbackPair.second;
                        _callback(_receivedMessage);
                    }
                }

            public:
                NetworkLayer() noexcept = default;
                virtual ~NetworkLayer() noexcept = default;

                /// @brief Send a message through the network
                /// @param message Message to be sent
                virtual void Send(const T &message) = 0;

                /// @brief Set a receiver callback
                /// @param object Object that owns the callback
                /// @param receiver Receiver callback to be called when a message has been received
                void SetReceiver(void* object, std::function<void(T)> receiver)
                {
                    mReceiverCallbacks[object] = receiver;
                }

                /// @brief Remove a receiver callback
                /// @param object Callback owner object
                void ResetReceiver(void* object)
                {
                    mReceiverCallbacks.erase(object);
                }
            };
        }
    }
}

#endif