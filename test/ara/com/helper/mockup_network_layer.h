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
                    // In the mockup network layer, the message payload is direcly forwarded to the receiver callback.
                    this->FireReceiverCallbacks(message.Payload());
                }
            };
        }
    }
}

#endif