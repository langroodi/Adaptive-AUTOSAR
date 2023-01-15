#ifndef SD_NETWORK_LAYER_H
#define SD_NETWORK_LAYER_H

#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/udp_client.h>
#include "../../helper/concurrent_queue.h"
#include "../../helper/network_layer.h"
#include "./someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery multicast network layer
                /// @note The class is not part of the ARA standard.
                class SdNetworkLayer : helper::NetworkLayer<SomeIpMessage>
                {
                private:
                    static const size_t cBufferSize{256};

                    helper::ConcurrentQueue<std::vector<uint8_t>> mSendingQueue;
                    AsyncBsdSocketLib::Poller *const mPoller;
                    AsyncBsdSocketLib::UdpClient mUdpSocket;

                    void onReceive();
                    void onSend();

                public:
                    /// @brief Constructor
                    /// @param poller BSD sockets poller
                    /// @param ipAddress Multicast group IPv4 address
                    /// @param port Multicast UDP port number
                    /// @throws std::runtime_error Throws when the UDP socket configuration failed
                    SdNetworkLayer(
                        AsyncBsdSocketLib::Poller *poller,
                        std::string ipAddress,
                        uint16_t port);

                    void Send(const SomeIpMessage &message) override;
                };
            }
        }
    }
}

#endif