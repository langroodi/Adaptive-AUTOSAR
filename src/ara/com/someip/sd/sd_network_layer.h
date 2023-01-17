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
            /// @brief SOME/IP service discovery namespace
            /// @note The namespace is not part of the ARA standard.
            namespace sd
            {
                /// @brief SOME/IP service discovery multicast network layer
                class SdNetworkLayer : public helper::NetworkLayer<SomeIpSdMessage>
                {
                private:
                    static const size_t cBufferSize;
                    static const std::string cAnyIpAddress;

                    const std::string cNicIpAddress;
                    const std::string cMulticastGroup;
                    const uint16_t cPort;
                    
                    helper::ConcurrentQueue<std::vector<uint8_t>> mSendingQueue;
                    AsyncBsdSocketLib::Poller *const mPoller;
                    AsyncBsdSocketLib::UdpClient mUdpSocket;

                    void onReceive();
                    void onSend();

                public:
                    /// @brief Constructor
                    /// @param poller BSD sockets poller
                    /// @param nicIpAddress Network interface controller IPv4 address
                    /// @param multicastGroup Multicast group IPv4 address
                    /// @param port Multicast UDP port number
                    /// @throws std::runtime_error Throws when the UDP socket configuration failed
                    SdNetworkLayer(
                        AsyncBsdSocketLib::Poller *poller,
                        std::string nicIpAddress,
                        std::string multicastGroup,
                        uint16_t port);

                    ~SdNetworkLayer() override;

                    void Send(const SomeIpSdMessage &message) override;
                };
            }
        }
    }
}

#endif