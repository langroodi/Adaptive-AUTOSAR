#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include <asyncbsdsocket/udp_client.h>

namespace AsyncBsdSocketLib
{
    class UdpUnicastTest : public testing::Test
    {
    protected:
        const std::string cLocalhost = "127.0.0.1";
        const uint16_t cAlicePort = 8090;
        const uint16_t cBobPort = 8091;

        UdpClient Alice;
        UdpClient Bob;
        bool SuccessfulSetup;

        UdpUnicastTest() : Alice(cLocalhost, cAlicePort),
                           Bob(cLocalhost, cBobPort)
        {
        }

        void SetUp()
        {
            SuccessfulSetup = Alice.TrySetup();

            if (SuccessfulSetup)
            {
                SuccessfulSetup = Bob.TrySetup();
            }
        }

        void TearDown()
        {
            Bob.TryClose();
            Alice.TryClose();
        }
    };

    TEST_F(UdpUnicastTest, CheckSetup)
    {
        EXPECT_TRUE(SuccessfulSetup);
    }

    TEST_F(UdpUnicastTest, Echo)
    {
        const std::size_t cBufferSize = 16;

        const std::array<uint8_t, cBufferSize> _sendBuffer{
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

        // Evaluate sending
        ssize_t _sentBytes =
            Alice.Send(_sendBuffer, cLocalhost, cBobPort);
        EXPECT_GT(_sentBytes, 0);

        std::array<uint8_t, cBufferSize> _receiveBuffer;
        std::string _senderIpAddress;
        uint16_t _senderPort;

        ssize_t _receivedBytes =
            Bob.Receive(_receiveBuffer, _senderIpAddress, _senderPort);

        // Evalute receiving and the sender
        EXPECT_GT(_receivedBytes, 0);
        EXPECT_EQ(_senderIpAddress, cLocalhost);
        EXPECT_EQ(_senderPort, cAlicePort);

        // Evaluate echo sending
        _sentBytes =
            Bob.Send(_receiveBuffer, _senderIpAddress, _senderPort);
        EXPECT_GT(_sentBytes, 0);

        _receivedBytes =
            Alice.Receive(_receiveBuffer, _senderIpAddress, _senderPort);

        // Evalute echo receiving and the echo sender
        EXPECT_GT(_receivedBytes, 0);
        EXPECT_EQ(_senderIpAddress, cLocalhost);
        EXPECT_EQ(_senderPort, cBobPort);

        // Evaluate echo data
        bool _areEqual =
            std::equal(
                std::cbegin(_sendBuffer),
                std::cend(_sendBuffer),
                std::cbegin(_receiveBuffer));
        EXPECT_TRUE(_areEqual);
    }
}