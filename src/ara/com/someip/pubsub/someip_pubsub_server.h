#ifndef SOMEIP_PUBSUB_SERVER
#define SOMEIP_PUBSUB_SERVER

#include "../../helper/finite_state_machine.h"
#include "../../helper/network_layer.h"
#include "../sd/someip_sd_message.h"
#include "../../entry/eventgroup_entry.h"
#include "../../option/ipv4_endpoint_option.h"
#include "./fsm/service_down_state.h"
#include "./fsm/notsubscribed_state.h"
#include "./fsm/subscribed_state.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                /// @brief SOME/IP publish/subscribe server
                class SomeIpPubSubServer
                {
                private:
                    helper::FiniteStateMachine<helper::PubSubState> mStateMachine;
                    helper::NetworkLayer<sd::SomeIpSdMessage> *mCommunicationLayer;
                    const uint16_t mServiceId;
                    const uint16_t mInstanceId;
                    const uint8_t mMajorVersion;
                    const uint16_t mEventgroupId;
                    const helper::Ipv4Address mEndpointIp;
                    const uint16_t mEndpointPort;
                    fsm::ServiceDownState mServiceDownState;
                    fsm::NotSubscribedState mNotSubscribedState;
                    fsm::SubscribedState mSubscribedState;

                    void onMessageReceived(sd::SomeIpSdMessage &&message);
                    void processEntry(const entry::EventgroupEntry *entry);

                public:
                    SomeIpPubSubServer() = delete;
                    ~SomeIpPubSubServer();

                    /// @brief Constructor
                    /// @param networkLayer Network communication abstraction layer
                    /// @param serviceId Service ID
                    /// @param instanceId Service instance ID
                    /// @param majorVersion Service major version
                    /// @param eventgroupId Service event-group ID
                    /// @param ipAddress Multicast IP address that clients should listen to for receiving events
                    /// @param port Multicast port number that clients should listen to for receiving events
                    SomeIpPubSubServer(
                        helper::NetworkLayer<sd::SomeIpSdMessage> *networkLayer,
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint16_t eventgroupId,
                        helper::Ipv4Address ipAddress,
                        uint16_t port);

                    /// @brief Start the server
                    void Start();

                    /// @brief Get the current server state
                    /// @returns Server machine state
                    helper::PubSubState GetState() const noexcept;

                    /// @brief Stop the server
                    void Stop();
                };
            }
        }
    }
}

#endif