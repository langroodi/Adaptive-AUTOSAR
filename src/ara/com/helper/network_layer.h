#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <vector>
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
                std::vector<std::function<void(T)>> mReceiverCallbacks;

            protected:
                /// @brief Fire all the set receiver callaback
                /// @param message Received message
                void FireReceiverCallbacks(T &&message)
                {
                    // Copy the received message
                    T _receivedMessage = message;

                    for (auto callback : mReceiverCallbacks)
                    {
                        callback(_receivedMessage);
                    }
                }

            public:
                NetworkLayer() noexcept = default;
                virtual ~NetworkLayer() noexcept = default;

                /// @brief Send a message through the network
                /// @param message Message to be sent
                virtual void Send(const T &message) = 0;

                /// @brief Set a receiver callback
                /// @param receiver Receiver callback to be called when a message has been received
                void SetReceiver(std::function<void(T)> receiver)
                {
                    mReceiverCallbacks.push_back(receiver);
                }
            };
        }
    }
}

#endif