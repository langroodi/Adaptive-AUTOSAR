#ifndef MOCKUP_NETWORK_LAYER_H
#define MOCKUP_NETWORK_LAYER_H

#include <utility>
#include "../../../../src/ara/com/helper/network_layer.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            template <typename T>
            class MockupNetworkLayer : public NetworkLayer<T>
            {
            public:
                MockupNetworkLayer() noexcept = default;

                virtual void Send(const T &message) override
                {
                    // Copy the received message
                    T _receivedMessage = message;
                    // In the mockup network layer, the message is direcly forwarded to the receiver callback.
                    this->ReceiverCallback(std::move(_receivedMessage));
                }
            };
        }
    }
}

#endif