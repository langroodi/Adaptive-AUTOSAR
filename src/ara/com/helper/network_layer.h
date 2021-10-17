#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <functional>

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
            protected:
                /// @brief Message receiver callaback
                std::function<void(T &&)> ReceiverCallback;

            public:
                NetworkLayer() noexcept = default;
                virtual ~NetworkLayer() noexcept = default;

                /// @brief Send a message through the network
                /// @param message Message to be sent
                virtual void Send(const T &message) = 0;

                /// @brief Set a receiver callback
                /// @param receiver Receiver callback to be called when a message has been received
                void SetReceiver(std::function<void(T &&)> receiver)
                {
                    ReceiverCallback = receiver;
                }
            };
        }
    }
}

#endif