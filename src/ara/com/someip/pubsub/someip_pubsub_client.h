#ifndef SOMEIP_PUBSUB_CLIENT
#define SOMEIP_PUBSUB_CLIENT

#include <condition_variable>
#include "../../entry/eventgroup_entry.h"
#include "../../helper/network_layer.h"
#include "../../helper/concurrent_queue.h"
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
                    helper::ConcurrentQueue<sd::SomeIpSdMessage> mMessageBuffer;
                    std::mutex mSubscriptionMutex;
                    std::unique_lock<std::mutex> mSubscriptionLock;
                    std::condition_variable mSubscriptionConditionVariable;
                    helper::NetworkLayer<sd::SomeIpSdMessage> *mCommunicationLayer;
                    uint8_t mCounter;
                    bool mValidNotify;

                    void onMessageReceived(sd::SomeIpSdMessage &&message);

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
                    void Subscribe(
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

                    /// @brief Try to wait unitl the server processes a subscription request
                    /// @param duration Waiting timeout in milliseconds
                    /// @param message The first processed subscription message in the buffer
                    /// @returns True, if the service offering is stopped before the timeout; otherwise false
                    bool TryGetProcessedSubscription(
                        int duration,
                        sd::SomeIpSdMessage &message);
                };
            }
        }
    }
}

#endif