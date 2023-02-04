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
            private:
                std::map<void *, std::function<void(T)>> mReceiverCallbacks;

            protected:
                /// @brief Fire all the set receiver callaback
                /// @param payload Received payload
                void FireReceiverCallbacks(const std::vector<uint8_t> &payload)
                {
                    for (auto objectCallbackPair : mReceiverCallbacks)
                    {
                        // Create the received message from the received payload
                        T _receivedMessage{T::Deserialize(payload)};

                        std::function<void(T)> _callback = objectCallbackPair.second;
                        _callback(std::move(_receivedMessage));
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
                void SetReceiver(void *object, std::function<void(T)> receiver)
                {
                    mReceiverCallbacks[object] = receiver;
                }
            };
        }
    }
}

#endif