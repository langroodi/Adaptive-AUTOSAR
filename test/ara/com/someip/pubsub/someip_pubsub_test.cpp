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
                    static const uint16_t cServiceId = 1;
                    static const uint16_t cInstanceId = 1;
                    static const uint8_t cMajorVersion = 1;
                    static const uint16_t cEventgroupId = 0;
                    static const uint16_t cPort = 9090;

                    helper::MockupNetworkLayer<sd::SomeIpSdMessage> mNetworkLayer;
                    helper::Ipv4Address mIpAddress;

                protected:
                    SomeIpPubSubServer Server;
                    SomeIpPubSubClient Client;

                    SomeIpPubSubTest() : mIpAddress(224, 0, 0, 0),
                                         Server(&mNetworkLayer,
                                                cServiceId,
                                                cInstanceId,
                                                cMajorVersion,
                                                cEventgroupId,
                                                mIpAddress,
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
            }
        }
    }
}