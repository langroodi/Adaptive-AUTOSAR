#ifndef MOCKUP_FIFO_LAYER_H
#define MOCKUP_FIFO_LAYER_H

#include "../../../../src/ara/exec/helper/fifo_layer.h"

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            template <typename T>
            class MockupFifoLayer : public FifoLayer<T>
            {
            public:
                MockupFifoLayer() noexcept = default;

                virtual void Send(const T &message) override
                {
                    if (this->ReceiverCallback)
                    {
                        this->ReceiverCallback(message);
                    }
                }
            };
        }
    }
}

#endif