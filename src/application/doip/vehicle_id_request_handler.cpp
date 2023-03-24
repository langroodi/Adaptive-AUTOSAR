#include "./vehicle_id_request_handler.h"

namespace application
{
    namespace doip
    {
        constexpr size_t VehicleIdRequestHandler::cMacAddressSize;
        const uint8_t VehicleIdRequestHandler::cFurtherAction;

        VehicleIdRequestHandler::VehicleIdRequestHandler(
            uint8_t protocolVersion,
            std::string &&vin,
            uint16_t logicalAddress,
            uint64_t eid,
            uint64_t gid) : mResponse(protocolVersion, std::move(vin), logicalAddress, convertToMacAddress(eid), convertToMacAddress(gid), cFurtherAction)
        {
        }

        std::array<uint8_t, VehicleIdRequestHandler::cMacAddressSize> VehicleIdRequestHandler::convertToMacAddress(
            uint64_t id)
        {
            uint64_t _mask{0xff};
            std::array<uint8_t, cMacAddressSize> _result;

            for (size_t i = 0; i < cMacAddressSize; ++i)
            {
                const uint64_t cOctet{id & _mask};
                _result[i] = static_cast<uint8_t>(cOctet >> (i * 8));
                _mask <<= i * 8;
            }

            return _result;
        }

        DoipLib::Message *VehicleIdRequestHandler::GetMessage()
        {
            return static_cast<DoipLib::Message *>(&mRequest);
        }

        bool VehicleIdRequestHandler::TryHandle(
            const DoipLib::Message *request,
            std::vector<uint8_t> &response) const
        {
            auto _vehicleIdRequest{
                dynamic_cast<const DoipLib::VehicleIdRequest *>(request)};

            if (_vehicleIdRequest)
            {
                mResponse.Serialize(response);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}