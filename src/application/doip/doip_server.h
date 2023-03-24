#ifndef DOIP_SERVER_H
#define DOIP_SERVER_H

#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/tcp_listener.h>
#include <doiplib/vehicle_id_response.h>
#include <doiplib/doip_controller.h>
#include "../../ara/com/helper/concurrent_queue.h"
#include "./doip_message_handler.h"

namespace application
{
    namespace doip
    {
        /// @brief Class to handle DoIP requests
        class DoipServer
        {
        private:
            static constexpr size_t cMacAddressSize{6};
            static constexpr size_t cDoipPacketSize{64};
            /// @note Due to no DoIP routing activation, the 'further action' is 0x00.
            static const uint8_t cFurtherAction{0x00};

            AsyncBsdSocketLib::Poller *const mPoller;
            DoipMessageHandler mMessageHandler;
            AsyncBsdSocketLib::TcpListener mListener;
            DoipLib::VehicleIdResponse mAnnouncement;
            DoipLib::DoipController mController;
            ara::com::helper::ConcurrentQueue<std::vector<uint8_t>> mSendQueue;

            static std::array<uint8_t, cMacAddressSize> convertToMacAddress(
                uint64_t id);

            void onAccept();
            void onReceive();
            void onSend();

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            /// @param curl Configured CURL instance for RESTful communication
            /// @param resourcesUrl Connected vehicle resources access RESTful URL
            /// @param ipAddress DoIP server listening IPv4 address
            /// @param port DoIP server TCP listening port number
            /// @param config DoIP controller configuration
            /// @param vin Vehicle Identification Number
            /// @param logicalAddress Vehicle logical address for DoIP communication
            /// @param eid DoIP communication entity ID
            /// @param gid DoIP communication group ID
            /// @throws std::runtime_error Throws when the TCP connection configuration failed
            DoipServer(
                AsyncBsdSocketLib::Poller *poller,
                helper::CurlWrapper *curl,
                std::string resourcesUrl,
                std::string ipAddress,
                uint16_t port,
                DoipLib::ControllerConfig &&config,
                std::string &&vin,
                uint16_t logicalAddress,
                uint64_t eid,
                uint64_t gid);

            ~DoipServer();
        };
    }
}

#endif