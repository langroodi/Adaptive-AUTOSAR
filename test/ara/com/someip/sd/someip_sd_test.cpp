#include <gtest/gtest.h>
#include "../../../../../src/ara/com/someip/sd/someip_sd_server.h"
#include "../../../../../src/ara/com/someip/sd/someip_sd_client.h"
#include "../../helper/mockup_network_layer.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                class SomeIpSdTest : public testing::Test
                {
                private:
                    static const uint16_t cServiceId = 1;
                    static const uint16_t cInstanceId = 1;
                    static const uint8_t cMajorVersion = 1;
                    static const uint32_t cMinorVersion = 0;
                    static const uint16_t cPort = 8080;
                    static const int cInitialDelayMin = 100;
                    static const int cInitialDelayMax = 200;
                    static const int cRepetitionBaseDelay = 200;
                    static const uint32_t cRepetitionMax = 2;
                    static const int cCycleOfferDelay = 100;

                    helper::MockupNetworkLayer<SomeIpSdMessage> mNetworkLayer;
                    helper::Ipv4Address mLocalhost;

                protected:
                    const int OfferWaitDuration;
                    const int OfferStopWaitDuration;

                    SomeIpSdServer Server;
                    SomeIpSdClient Client;

                    SomeIpSdTest() : mNetworkLayer(),
                                     mLocalhost(127, 0, 0, 1),
                                     Server(
                                         &mNetworkLayer,
                                         cServiceId,
                                         cInstanceId,
                                         cMajorVersion,
                                         cMinorVersion,
                                         mLocalhost,
                                         cPort,
                                         cInitialDelayMin,
                                         cInitialDelayMax,
                                         cRepetitionBaseDelay,
                                         cCycleOfferDelay,
                                         cRepetitionMax),
                                     Client(
                                         &mNetworkLayer,
                                         cServiceId,
                                         cInitialDelayMin,
                                         cInitialDelayMax,
                                         cRepetitionBaseDelay,
                                         cRepetitionMax),
                                     OfferWaitDuration{static_cast<int>(
                                         // Initial wait phase delay
                                         (cInitialDelayMax +
                                          // Summation of all the repetition phase delays
                                          cRepetitionBaseDelay * (std::pow(2, cRepetitionMax) - 1) +
                                          // Main main first cycle delay
                                          cCycleOfferDelay) *
                                         // Apply minimum Nyquist–Shannon margin (make the duration twice longer)
                                         2)},
                                     OfferStopWaitDuration{static_cast<int>(
                                         // Main main first cycle delay
                                         cCycleOfferDelay *
                                         // Apply maximum Nyquist–Shannon margin (make the duration ten times longer)
                                         10)}
                    {
                    }
                };

                TEST_F(SomeIpSdTest, ServerStart)
                {
                    const helper::SdServerState cServerStoppedState =
                        helper::SdServerState::NotReady;

                    EXPECT_EQ(Server.GetState(), cServerStoppedState);
                    EXPECT_NO_THROW(Server.Start());
                    EXPECT_THROW(Server.Start(), std::logic_error);
                }

                TEST_F(SomeIpSdTest, ClientStart)
                {
                    const helper::SdClientState cClientStoppedState =
                        helper::SdClientState::ServiceNotSeen;

                    EXPECT_EQ(Client.GetState(), cClientStoppedState);
                    EXPECT_NO_THROW(Client.Start());
                    EXPECT_THROW(Client.Start(), std::logic_error);
                }

                TEST_F(SomeIpSdTest, StartScenario)
                {
                    const helper::SdClientState cServiceReadyState =
                        helper::SdClientState::ServiceReady;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(OfferWaitDuration);

                    EXPECT_EQ(Client.GetState(), cServiceReadyState);
                }

                TEST_F(SomeIpSdTest, ClientStopScenario)
                {
                    const helper::SdClientState cServiceSeenState =
                        helper::SdClientState::ServiceSeen;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(OfferWaitDuration);
                    Client.Stop();

                    EXPECT_EQ(Client.GetState(), cServiceSeenState);
                }

                TEST_F(SomeIpSdTest, ServerStopScenario)
                {
                    const helper::SdClientState cServiceStoppedState =
                        helper::SdClientState::Stopped;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(OfferWaitDuration);
                    Server.Stop();
                    Client.TryWaitUntiServiceOfferStopped(OfferStopWaitDuration);

                    EXPECT_EQ(Client.GetState(), cServiceStoppedState);
                }

                TEST_F(SomeIpSdTest, StopScenario)
                {
                    const helper::SdClientState cServiceNotSeen =
                        helper::SdClientState::ServiceNotSeen;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(OfferWaitDuration);
                    Client.Stop();
                    Server.Stop();
                    Client.TryWaitUntiServiceOfferStopped(OfferStopWaitDuration);

                    EXPECT_EQ(Client.GetState(), cServiceNotSeen);
                }
            }
        }
    }
}