#ifndef DOIP_SERVER_H
#define DOIP_SERVER_H

#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/tcp_listener.h>
#include <doiplib/doip_controller.h>
#include "../../ara/com/helper/concurrent_queue.h"
#include "./vehicle_id_request_handler.h"
#include "./diag_message_handler.h"

namespace application
{
    namespace doip
    {
        /// @brief Class to handle DoIP requests
        class DoipServer
        {
        public:
            /// @brief Maximum DoIP packet size
            static constexpr size_t cDoipPacketSize{64};

        private:
            AsyncBsdSocketLib::Poller *const mPoller;
            VehicleIdRequestHandler mVehicleIdRequestHandler;
            DiagMessageHandler mDiagMessageHandler;
            AsyncBsdSocketLib::TcpListener mListener;
            DoipLib::DoipController mController;
            ara::com::helper::ConcurrentQueue<std::vector<uint8_t>> mSendQueue;

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