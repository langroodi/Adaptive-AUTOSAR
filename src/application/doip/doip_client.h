#ifndef DOIP_CLIENT_H
#define DOIP_CLIENT_H

#include <functional>
#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/tcp_client.h>
#include "../../ara/core/optional.h"
#include "../../ara/com/helper/concurrent_queue.h"

namespace application
{
    namespace doip
    {
        /// @brief A class to send DoIP requests to a DoIP server
        /// @see DoipServer
        class DoipClient
        {
        private:
            static constexpr size_t cDoipPacketSize{64};

            AsyncBsdSocketLib::Poller *const mPoller;
            AsyncBsdSocketLib::TcpClient mClient;
            const uint8_t cProtocolVersion;
            const std::function<void(std::vector<uint8_t> &&)> mCallback;
            ara::com::helper::ConcurrentQueue<std::vector<uint8_t>> mSendQueue;
            ara::core::Optional<uint16_t> mLogicalAddress;

            void handleVehicleIdResponse(
                const std::vector<uint8_t> &serializedMessage);

            void handleDiagMessageAck(
                const std::vector<uint8_t> &serializedMessage) const;

            void onSend();
            void onReceive();

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            /// @param ipAddress DoIP server IPv4 address
            /// @param port DoIP server TCP listening port number
            /// @param protocolVersion DoIP protocol version
            /// @param callback Callback to be invoked while receiving diagnostic message ACKs
            /// @throws std::runtime_error Throws when the TCP connection configuration failed
            DoipClient(
                AsyncBsdSocketLib::Poller *poller,
                std::string ipAddress,
                uint16_t port,
                uint8_t protocolVersion,
                std::function<void(std::vector<uint8_t> &&)> &&callback);

            /// @brief Try to send a diagnostic message to the DoIP server asynchronously
            /// @param userData Diagnostic message user data (UDS message payload)
            /// @return True if the message is successfully queued for sending; otherwise false
            bool TrySendDiagMessage(std::vector<uint8_t> &&userData);

            ~DoipClient();
        };
    }
}

#endif