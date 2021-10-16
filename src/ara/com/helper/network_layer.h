#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <functional>
#include "./ipv4_address.h"

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
                /// @brief Listening IP address
                Ipv4Address IpAddress;

                /// @brief Listening UDP port
                uint16_t Port;

                /// @brief Message receiver callaback
                std::function<void(T)> ReceiverCallback;

                /// @brief Constructor
                /// @param ipAddress Listening IP address
                /// @param port Listening UDP port
                NetworkLayer(Ipv4Address ipAddress, uint16_t port) noexcept : IpAddress{ipAddress},
                                                                              Port{port}
                {
                }

                NetworkLayer() = delete;
                virtual ~NetworkLayer() noexcept = default;

            public:
                /// @brief Send a message through the network
                /// @param message Message to be sent
                virtual void Send(T message) = 0;

                /// @brief Set a receiver callback
                /// @param receiver Receiver callback to be called when a message has been received
                void SetReceiver(std::function<void(T)> receiver)
                {
                    ReceiverCallback = receiver;
                }
            };
        }
    }
}

#endif