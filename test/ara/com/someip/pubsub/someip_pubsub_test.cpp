#include <gtest/gtest.h>
#include "../../../../../src/ara/com/someip/pubsub/someip_pubsub_server.h"
#include "../../../../../src/ara/com/someip/pubsub/someip_pubsub_client.h"
#include "../../helper/mockup_network_layer.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                class SomeIpPubSubTest : public testing::Test
                {
                private:
                    static const uint16_t cCounter = 0;

                    helper::MockupNetworkLayer<sd::SomeIpSdMessage> mNetworkLayer;

                protected:
                    static const uint16_t cServiceId = 1;
                    static const uint16_t cInstanceId = 1;
                    static const uint8_t cMajorVersion = 1;
                    static const uint16_t cEventgroupId = 0;
                    static const uint16_t cPort = 10001;
                    static const int cWaitingDuration = 100;

                    SomeIpPubSubServer Server;
                    SomeIpPubSubClient Client;

                    SomeIpPubSubTest() : Server(&mNetworkLayer,
                                                cServiceId,
                                                cInstanceId,
                                                cMajorVersion,
                                                cEventgroupId,
                                                helper::Ipv4Address(224, 0, 0, 0),
                                                cPort),
                                         Client(
                                             &mNetworkLayer,
                                             cCounter)
                    {
                    }
                };

                TEST_F(SomeIpPubSubTest, ServerInitialState)
                {
                    const helper::PubSubState cExpectedState =
                        helper::PubSubState::ServiceDown;

                    helper::PubSubState _actualState = Server.GetState();

                    EXPECT_EQ(cExpectedState, _actualState);
                }

                TEST_F(SomeIpPubSubTest, NoServerRunning)
                {
                    const int cMinimalDuration = 1;
                    sd::SomeIpSdMessage _message;

                    EXPECT_FALSE(
                        Client.TryGetProcessedSubscription(
                            cMinimalDuration, _message));
                }

                TEST_F(SomeIpPubSubTest, ServerStart)
                {
                    const helper::PubSubState cExpectedState =
                        helper::PubSubState::NotSubscribed;

                    Server.Start();
                    helper::PubSubState _actualState = Server.GetState();

                    EXPECT_EQ(cExpectedState, _actualState);
                }

                TEST_F(SomeIpPubSubTest, AcknowlegeScenario)
                {
                    const helper::PubSubState cExpectedState =
                        helper::PubSubState::Subscribed;
                    const option::OptionType cExpectedOptionType =
                        option::OptionType::IPv4Multicast;
                    const uint16_t cExpectedPort{cPort};
                    sd::SomeIpSdMessage _message;

                    Server.Start();
                    Client.Subscribe(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);
                    bool _succeed =
                        Client.TryGetProcessedSubscription(cWaitingDuration, _message);

                    EXPECT_TRUE(_succeed);

                    auto _eventgroupEntry =
                        dynamic_cast<entry::EventgroupEntry *>(
                            _message.Entries().at(0).get());

                    EXPECT_GT(_eventgroupEntry->TTL(), 0);
                    EXPECT_EQ(_eventgroupEntry->FirstOptions().size(), 1);

                    auto _endpointOption =
                        dynamic_cast<option::Ipv4EndpointOption *>(
                            _eventgroupEntry->FirstOptions().at(0).get());

                    EXPECT_EQ(_endpointOption->Type(), cExpectedOptionType);
                    EXPECT_EQ(_endpointOption->Port(), cExpectedPort);

                    EXPECT_EQ(cExpectedState, Server.GetState());
                }

                TEST_F(SomeIpPubSubTest, NegativeAcknowlegeScenario)
                {
                    const helper::PubSubState cExpectedState =
                        helper::PubSubState::ServiceDown;
                    sd::SomeIpSdMessage _message;

                    Client.Subscribe(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);
                    bool _succeed =
                        Client.TryGetProcessedSubscription(cWaitingDuration, _message);

                    EXPECT_TRUE(_succeed);

                    auto _eventgroupEntry =
                        dynamic_cast<entry::EventgroupEntry *>(
                            _message.Entries().at(0).get());

                    EXPECT_EQ(_eventgroupEntry->TTL(), 0);
                    EXPECT_EQ(cExpectedState, Server.GetState());
                }

                TEST_F(SomeIpPubSubTest, UnsubscriptionScenario)
                {
                    const helper::PubSubState cExpectedState =
                        helper::PubSubState::NotSubscribed;
                    sd::SomeIpSdMessage _message;

                    Server.Start();
                    Client.Subscribe(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);
                    bool _succeed =
                        Client.TryGetProcessedSubscription(cWaitingDuration, _message);

                    EXPECT_TRUE(_succeed);

                    Client.Unsubscribe(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                    EXPECT_EQ(cExpectedState, Server.GetState());
                }
            }
        }
    }
}