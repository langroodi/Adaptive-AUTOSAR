#ifndef OBD_TO_DOIP_CONVERTER_H
#define OBD_TO_DOIP_CONVERTER_H

#include <obdemulator/obd_service.h>
#include "./doip_client.h"

namespace application
{
    namespace doip
    {
        /// @brief A OBD service class to handle OBD queries asynchronously
        /// @details The class based on the queried PID may convert the query to a UDS message and send it via DoIP to a UDS server.
        class ObdToDoipConverter : public ObdEmulator::ObdService
        {
        private:
            // Show current data mode
            static const uint8_t cService{0x01};
            static const uint8_t cSid{0x22};
            static const uint8_t cProtocolVersion{2};

            DoipClient mClient;

            static bool tryParseUdsData(
                std::vector<uint8_t> &&udsResponse,
                uint8_t &pid,
                std::vector<uint8_t> &obdData);

            void onUdsDataReceived(
                std::vector<uint8_t> &&receivedUdsData) const;

            bool trySendAsDoip(uint8_t pid);

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            /// @param ipAddress DoIP server IPv4 address
            /// @param port DoIP server TCP listening port number
            /// @throws std::runtime_error Throws when the DoIP client initialization failed
            ObdToDoipConverter(
                AsyncBsdSocketLib::Poller *poller,
                std::string ipAddress,
                uint16_t port);

            bool TryGetResponse(
                const std::vector<uint8_t> &pid,
                std::vector<uint8_t> &response) const override;

            bool TryGetResponseAsync(const std::vector<uint8_t> &pid) override;
        };
    }
}

#endif