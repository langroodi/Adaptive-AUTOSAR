#include <gtest/gtest.h>
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
                    SomeIpPubSubClient Client;

                    SomeIpPubSubTest() : mNetworkLayer(),
                                         Client(
                                             &mNetworkLayer,
                                             cCounter)
                    {
                    }
                };

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