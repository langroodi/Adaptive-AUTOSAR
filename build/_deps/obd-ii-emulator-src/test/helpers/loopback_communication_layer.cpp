#include <stdexcept>
#include <obdemulator/can_driver.h>
#include "./loopback_communication_layer.h"

namespace ObdEmulator
{
    namespace Helpers
    {
        LoopbackCommunicationLayer::LoopbackCommunicationLayer(
            const CanDriver *canDriver) noexcept : mCanDriver(canDriver),
                                                   mCanFrame{nullptr}
        {
        }

        bool LoopbackCommunicationLayer::TryStart(std::vector<uint8_t> &&configuration)
        {
            return true;
        }

        bool LoopbackCommunicationLayer::TrySendAsync(std::vector<uint8_t> &&data)
        {
            if (mCanFrame)
            {
                delete mCanFrame;
            }
            mCanFrame = new CanFrame{mCanDriver->Deserialize(data)};
            return true;
        }

        CanFrame LoopbackCommunicationLayer::Send(const CanFrame &queryFrame)
        {
            std::vector<uint8_t> _queryData{mCanDriver->Serialize(queryFrame)};
            std::vector<uint8_t> _responseData;
            bool _succeed{Callback(std::move(_queryData), _responseData)};
            if (_succeed)
            {
                CanFrame _result{mCanDriver->Deserialize(_responseData)};
                return _result;
            }
            else
            {
                throw std::runtime_error("The query is not handled.");
            }
        }

        void LoopbackCommunicationLayer::SendAsync(const CanFrame &queryFrame)
        {
            std::vector<uint8_t> _queryData{mCanDriver->Serialize(queryFrame)};
            std::vector<uint8_t> _responseData;

            AsyncCallback(std::move(_queryData));
        }

        bool LoopbackCommunicationLayer::TryGetLastReceivedCanFrame(
            const CanFrame *&receivedCanFrame) const noexcept
        {
            if (mCanFrame)
            {
                receivedCanFrame = mCanFrame;
                return true;
            }
            else
            {
                return false;
            }
        }

        bool LoopbackCommunicationLayer::TryStop()
        {
            return true;
        }

        LoopbackCommunicationLayer::~LoopbackCommunicationLayer()
        {
            TryStop();

            if (mCanFrame)
            {
                delete mCanFrame;
            }
        }
    }
}