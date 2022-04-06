#include <gtest/gtest.h>
#include <asyncbsdsocket/tcp_client.h>
#include <asyncbsdsocket/tcp_listener.h>
#include <asyncbsdsocket/udp_client.h>

namespace AsyncBsdSocketLib
{
    TEST(NetworkSocketTest, TcpClientConstructor)
    {
        const std::string cIpAddress = "127.0.0.1";
        const uint16_t cPort = 8080;

        TcpClient _tcpClient(cIpAddress, cPort);

        EXPECT_FALSE(_tcpClient.IsConnected());
    }

    TEST(NetworkSocketTest, TcpListenerConstructor)
    {
        const uint16_t cPort = 8080;
        const int cConnection = -1;

        TcpListener _tcpListener(cPort);

        EXPECT_EQ(_tcpListener.Connection(), cConnection);
    }

    TEST(NetworkSocketTest, UdpClientConstructor)
    {
        const std::string cIpAddress = "127.0.0.1";
        const uint16_t cPort = 8080;
        const int cDescriptor = -1;

        UdpClient _udpClient(cIpAddress, cPort);

        EXPECT_EQ(_udpClient.Descriptor(), cDescriptor);
    }
}