#ifndef SOMEIP_PUBSUB_CLIENT
#define SOMEIP_PUBSUB_CLIENT

#include "../../entry/eventgroup_entry.h"
#include "../../helper/network_layer.h"
#include "../sd/someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                /// @brief SOME/IP publish/subscribe client
                class SomeIpPubSubClient
                {
                private:
                    helper::NetworkLayer<sd::SomeIpSdMessage> *mCommunicationLayer;
                    uint8_t mCounter;

                public:
                    SomeIpPubSubClient() = delete;
                    ~SomeIpPubSubClient();

                    /// @brief Constructor
                    /// @param networkLayer Network communication abstraction layer
                    /// @param counter Counter to make the client distinguishable among other subscribers
                    SomeIpPubSubClient(
                        helper::NetworkLayer<sd::SomeIpSdMessage> *networkLayer,
                        uint8_t counter);

                    /// @brief Subscribe to an event-group
                    /// @param serviceId Service in interest ID
                    /// @param instanceId Service in interest instance ID
                    /// @param majorVersion Service in interest major version
                    /// @param eventgroupId Event-group in interest ID
                    /// @returns Server response which can be subscription ACK or NACK
                    sd::SomeIpSdMessage Subscribe(
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint16_t eventgroupId);

                    /// @brief Unsubscribe from a subscribed event-group
                    /// @param serviceId Service in interest ID
                    /// @param instanceId Service in interest instance ID
                    /// @param majorVersion Service in interest major version
                    /// @param eventgroupId Event-group in interest ID
                    void Unsubscribe(
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint16_t eventgroupId);
                };
            }
        }
    }
}

#endif