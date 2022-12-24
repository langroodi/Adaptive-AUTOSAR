#include <utility>
#include "doiplib/doip_controller.h"
#include "doiplib/generic_nack.h"
#include "doiplib/convert.h"

namespace DoipLib
{
    DoipController::DoipController(
        ControllerConfig &&configuration) : mConfiguration{std::move(configuration)},
                                            mTimer(mConfiguration.doIPInitialVehicleAnnouncementTime, mConfiguration.doIPVehicleAnnouncementInterval, mConfiguration.doIPVehicleAnnouncementCount)
    {
    }

    void DoipController::CreateGenericNack(
        GenericNackType nackCode, std::vector<uint8_t> &response) const
    {
        GenericNack _genericNack(mConfiguration.protocolVersion, nackCode);
        _genericNack.Serialize(response);
    }

    void DoipController::Register(
        PayloadType payloadType, MessageHandler *messageHandler)
    {
        mHandlers[payloadType] = messageHandler;
    }

    bool DoipController::TryHandle(
        const std::vector<uint8_t> &request,
        std::vector<uint8_t> &response) const
    {
        bool _result;

        if (request.size() <= mConfiguration.doipMaxRequestBytes)
        {
            if (request.size() > cPayloadTypeOffset)
            {
                std::size_t _offset{cPayloadTypeOffset};
                auto _payloadType{
                    Convert::ToEnum<PayloadType>(request, _offset)};

                auto _messageHandlerItr{mHandlers.find(_payloadType)};
                if (_messageHandlerItr != mHandlers.end())
                {
                    MessageHandler *_messageHandler{_messageHandlerItr->second};
                    Message *_message{_messageHandler->GetMessage()};
                    GenericNackType _nackCode;
                    bool _succeed{_message->TryDeserialize(request, _nackCode)};

                    if (_succeed)
                    {
                        _result = _messageHandler->TryHandle(_message, response);
                    }
                    else
                    {
                        CreateGenericNack(_nackCode, response);
                        _result = true;
                    }
                }
                else
                {
                    CreateGenericNack(GenericNackType::UnsupportedPayloadType, response);
                    _result = true;
                }
            }
            else
            {
                CreateGenericNack(GenericNackType::InvalidPayloadLength, response);
                _result = true;
            }
        }
        else
        {
            CreateGenericNack(GenericNackType::ToolLargeMessage, response);
            _result = true;
        }

        return _result;
    }

    void DoipController::StartAnnoucement(std::function<void()> &&callback)
    {
        mTimer.Start(std::move(callback));
    }
}