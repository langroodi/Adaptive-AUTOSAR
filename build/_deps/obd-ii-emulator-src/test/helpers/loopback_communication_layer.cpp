#include <stdexcept>
#include <obdemulator/can_driver.h>
#include "./loopback_communication_layer.h"

namespace ObdEmulator
{
    namespace Helpers
    {
        LoopbackCommunicationLayer::LoopbackCommunicationLayer(
            const CanDriver *canDriver) noexcept : mCanDriver(canDriver)
        {
        }

        bool LoopbackCommunicationLayer::TryStart(std::vector<uint8_t> &&configuration)
        {
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

        bool LoopbackCommunicationLayer::TryStop()
        {
            return true;
        }

        LoopbackCommunicationLayer::~LoopbackCommunicationLayer()
        {
            TryStop();
        }
    }
}