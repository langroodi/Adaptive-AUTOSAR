#ifndef VEHICLE_ID_REQUEST_HANDLER_H
#define VEHICLE_ID_REQUEST_HANDLER_H

#include <doiplib/message_handler.h>
#include <doiplib/vehicle_id_request.h>
#include <doiplib/vehicle_id_response.h>

namespace application
{
    namespace doip
    {
        /// @brief A class to handle vehicle ID requests via DoIP
        class VehicleIdRequestHandler : public DoipLib::MessageHandler
        {
        private:
            static constexpr size_t cMacAddressSize{6};
            /// @note Due to no DoIP routing activation, the 'further action' is 0x00.
            static const uint8_t cFurtherAction{0x00};

            DoipLib::VehicleIdRequest mRequest;
            DoipLib::VehicleIdResponse mResponse;

            static std::array<uint8_t, cMacAddressSize> convertToMacAddress(
                uint64_t id);

        public:
            /// @brief Constructor
            /// @param protocolVersion DoIP protocol version
            /// @param vin Vehicle Identification Number
            /// @param logicalAddress Vehicle logical address for DoIP communication
            /// @param eid DoIP communication entity ID
            /// @param gid DoIP communication group ID
            VehicleIdRequestHandler(
                uint8_t protocolVersion,
                std::string &&vin,
                uint16_t logicalAddress,
                uint64_t eid,
                uint64_t gid);

            DoipLib::Message *GetMessage() override;

            bool TryHandle(
                const DoipLib::Message *request,
                std::vector<uint8_t> &response) const override;
        };
    }
}

#endif