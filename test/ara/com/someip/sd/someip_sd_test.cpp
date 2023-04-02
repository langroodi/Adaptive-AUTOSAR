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
                    static const int cInitialDelayMin = 100;
                    static const int cInitialDelayMax = 200;
                    static const int cRepetitionBaseDelay = 200;
                    static const uint32_t cRepetitionMax = 2;
                    static const int cCycleOfferDelay = 100;

                    helper::MockupNetworkLayer<SomeIpSdMessage> mNetworkLayer;
                    helper::Ipv4Address mLocalhost;

                protected:
                    static const std::string cIpAddress;
                    static const uint16_t cPort;

                    const int WaitDuration;

                    SomeIpSdServer Server;
                    SomeIpSdClient Client;

                    SomeIpSdTest() : mLocalhost(cIpAddress),
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
                                     WaitDuration{static_cast<int>(
                                         // Initial wait phase delay
                                         (cInitialDelayMax +
                                          // Summation of all the repetition phase delays
                                          cRepetitionBaseDelay * (std::pow(2, cRepetitionMax) - 1) +
                                          // Main main first cycle delay
                                          cCycleOfferDelay) *
                                         // Apply high Nyquist–Shannon margin (make the duration twice longer)
                                         10)}
                    {
                    }
                };

                const std::string SomeIpSdTest::cIpAddress{"127.0.0.1"};
                const uint16_t SomeIpSdTest::cPort{8080};

                TEST_F(SomeIpSdTest, ServerStart)
                {
                    const helper::SdServerState cNotReadyState =
                        helper::SdServerState::NotReady;

                    EXPECT_EQ(Server.GetState(), cNotReadyState);
                    EXPECT_NO_THROW(Server.Start());
                    EXPECT_THROW(Server.Start(), std::logic_error);
                }

                TEST_F(SomeIpSdTest, ClientStart)
                {
                    const helper::SdClientState cServiceNotSeenState =
                        helper::SdClientState::ServiceNotSeen;

                    EXPECT_EQ(Client.GetState(), cServiceNotSeenState);
                    EXPECT_NO_THROW(Client.Start());
                    EXPECT_THROW(Client.Start(), std::logic_error);

                    std::string _ipAddress;
                    uint16_t _port;
                    EXPECT_FALSE(Client.TryGetOfferedEndpoint(_ipAddress, _port));
                }

                TEST_F(SomeIpSdTest, StartScenario)
                {
                    const helper::SdClientState cServiceReadyState =
                        helper::SdClientState::ServiceReady;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(WaitDuration);

                    EXPECT_EQ(Client.GetState(), cServiceReadyState);

                    std::string _ipAddress;
                    uint16_t _port;
                    EXPECT_TRUE(Client.TryGetOfferedEndpoint(_ipAddress, _port));
                    EXPECT_EQ(cIpAddress, _ipAddress);
                    EXPECT_EQ(cPort, _port);
                }

                TEST_F(SomeIpSdTest, ClientStopScenario)
                {
                    const helper::SdClientState cServiceSeenState =
                        helper::SdClientState::ServiceSeen;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(WaitDuration);
                    Client.Stop();
                    Client.TryWaitUntiServiceOffered(WaitDuration);

                    EXPECT_EQ(Client.GetState(), cServiceSeenState);
                }

                TEST_F(SomeIpSdTest, ServerStopScenario)
                {
                    const helper::SdClientState cStoppedState =
                        helper::SdClientState::Stopped;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(WaitDuration);
                    Server.Stop();
                    Client.TryWaitUntiServiceOfferStopped(WaitDuration);

                    EXPECT_EQ(Client.GetState(), cStoppedState);
                }

                TEST_F(SomeIpSdTest, StopScenario)
                {
                    const helper::SdClientState cServiceNotSeen =
                        helper::SdClientState::ServiceNotSeen;

                    Server.Start();
                    Client.Start();
                    Client.TryWaitUntiServiceOffered(WaitDuration);
                    Server.Stop();
                    Client.TryWaitUntiServiceOfferStopped(WaitDuration);
                    Client.Stop();
                    Client.TryWaitUntiServiceOfferStopped(WaitDuration);

                    EXPECT_EQ(Client.GetState(), cServiceNotSeen);
                }
            }
        }
    }
}