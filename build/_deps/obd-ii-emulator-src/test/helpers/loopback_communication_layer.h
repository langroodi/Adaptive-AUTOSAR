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
            CanFrame *mCanFrame;

        public:
            LoopbackCommunicationLayer(const CanDriver *canDriver) noexcept;
            ~LoopbackCommunicationLayer();

            bool TryStart(std::vector<uint8_t> &&configuration) override;

            bool TrySendAsync(std::vector<uint8_t> &&data) override;

            /// @brief Invoke the mocked communication by sending a CAN query frame
            /// @param queryFrame CAN query frame to be sent
            /// @return CAN result frame
            CanFrame Send(const CanFrame &queryFrame);

            /// @brief Invoke the mocked communication by sending a CAN query frame
            /// @param queryFrame CAN query frame to be sent
            void SendAsync(const CanFrame &queryFrame);

            bool TryGetLastReceivedCanFrame(
                const CanFrame *&receivedCanFrame) const noexcept;

            bool TryStop() override;
        };
    }
}

#endif