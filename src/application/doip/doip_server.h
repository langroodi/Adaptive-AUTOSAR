#ifndef DOIP_SERVER_H
#define DOIP_SERVER_H

#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/tcp_listener.h>
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
            static constexpr size_t cDoipPacketSize{64};

            AsyncBsdSocketLib::Poller *const mPoller;
            DoipMessageHandler mMessageHandler;
            AsyncBsdSocketLib::TcpListener mListener;
            DoipLib::DoipController mController;
            ara::com::helper::ConcurrentQueue<std::vector<uint8_t>> mSendQueue;

            void onAccept();
            void onReceive();
            void onSend();

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            /// @param ipAddress DoIP server listening IPv4 address
            /// @param port DoIP server TCP listening port number
            /// @param config DoIP controller configuration
            /// @param curl Configured CURL instance for RESTful communication
            /// @param resourcesUrl Connected vehicle resources access RESTful URL
            /// @throws std::runtime_error Throws when the TCP connection configuration failed
            DoipServer(
                AsyncBsdSocketLib::Poller *poller,
                helper::CurlWrapper *curl,
                std::string resourcesUrl,
                std::string ipAddress,
                uint16_t port,
                DoipLib::ControllerConfig &&config);

            ~DoipServer();
        };
    }
}

#endif