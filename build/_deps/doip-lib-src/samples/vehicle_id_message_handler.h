#ifndef VEHICLE_ID_MESSAGE_HANDLER_H
#define VEHICLE_ID_MESSAGE_HANDLER_H

#include "../include/doiplib/message_handler.h"
#include "../include/doiplib/vehicle_id_response.h"
#include "../include/doiplib/eid_vehicle_id_request.h"

namespace DoipLib
{
    class VehicleIdMessageHandler : public MessageHandler
    {
    private:
        static constexpr std::size_t cIdSize{6};
        static const uint8_t cFurtherAction{0x00};

        VehicleIdResponse mResponse;
        EidVehicleIdRequest mRequest;

    public:
        VehicleIdMessageHandler(
            uint8_t protocolVersion,
            std::string &&vin,
            uint16_t logicalAddress,
            std::array<uint8_t, cIdSize> eid,
            std::array<uint8_t, cIdSize> gid);

        Message *GetMessage() override;

        bool TryHandle(
            const Message *request, std::vector<uint8_t> &response) const override;
    };
}

#endif