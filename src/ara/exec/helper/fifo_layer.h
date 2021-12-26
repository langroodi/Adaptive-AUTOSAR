#ifndef FIFO_LAYER_H
#define FIFO_LAYER_H

#include <functional>

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            /// @brief FIFO (named pipe) interprocess communication abstraction layer
            /// @tparam T Message type
            template <typename T>
            class FifoLayer
            {
            protected:
                /// @brief Callaback to be invoked when a message is received
                std::function<void(T)> ReceiverCallback;

            public:
                FifoLayer() noexcept = default;
                virtual ~FifoLayer() noexcept = default;

                /// @brief Send a message through the network
                /// @param message Message to be sent
                virtual void Send(const T &message) = 0;

                /// @brief Set a receiver callback
                /// @param receiver Receiver callback to be called when a message has been received
                inline void SetReceiver(std::function<void(T)> receiver)
                {
                    ReceiverCallback = receiver;
                }
            };
        }
    }
}

#endif