#ifndef DUMMY_MESSAGE_HANDLER_H
#define DUMMY_MESSAGE_HANDLER_H

#include <doiplib/message_handler.h>
#include <doiplib/alive_check_request.h>
#include <doiplib/alive_check_response.h>

namespace DoipLib
{
    class DummyMessageHandler : public MessageHandler
    {
    private:
        static const uint8_t cProtocolVersion{2};
        
        const AliveCheckResponse mResponse;
        AliveCheckRequest mRequest;

    public:
        explicit DummyMessageHandler(uint16_t sourceAddress) noexcept : mResponse(cProtocolVersion, sourceAddress)
        {
        }

        virtual Message *GetMessage() override
        {
            auto _result{static_cast<Message *>(&mRequest)};
            return _result;
        }

        virtual bool TryHandle(
            const Message *request, std::vector<uint8_t> &response) const override
        {
            mResponse.Serialize(response);
            return true;
        }
    };
}

#endif