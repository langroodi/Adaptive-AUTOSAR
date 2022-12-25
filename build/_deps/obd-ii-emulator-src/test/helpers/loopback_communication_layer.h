#ifndef LOOPBACK_COMMUNICATION_LAYER_H
#define LOOPBACK_COMMUNICATION_LAYER_H

#include <obdemulator/communication_layer.h>
#include <obdemulator/can_frame.h>

namespace ObdEmulator
{
    namespace Helpers
    {
        class LoopbackCommunicationLayer : public CommunicationLayer
        {
        private:
            const CanDriver *mCanDriver;

        public:
            LoopbackCommunicationLayer(const CanDriver *canDriver) noexcept;
            ~LoopbackCommunicationLayer();

            bool TryStart(std::vector<uint8_t> &&configuration) override;

            CanFrame Send(const CanFrame &queryFrame);

            bool TryStop() override;
        };
    }
}

#endif