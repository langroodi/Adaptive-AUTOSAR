#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include <asyncbsdsocket/tcp_client.h>
#include <asyncbsdsocket/tcp_listener.h>

namespace AsyncBsdSocketLib
{
    class TcpCommunicationTest : public testing::Test
    {
    private:
        const std::string cLocalhost = "127.0.0.1";
        const uint16_t cPort = 8080;

    protected:
        TcpListener Server;
        TcpClient Client;

        TcpCommunicationTest() : Server(cLocalhost, cPort),
                                 Client(cLocalhost, cPort)
        {
        }

        void SetUp()
        {
            bool _succeed = Server.TrySetup();

            if (_succeed)
            {
                _succeed = Client.TrySetup();

                if (_succeed)
                {
                    _succeed = Client.TryConnect();

                    if (_succeed)
                    {
                        Server.TryAccept();
                    }
                }
            }
        }

        void TearDown()
        {
            Client.TryClose();
            Server.TryClose();
        }
    };

    TEST_F(TcpCommunicationTest, Connection)
    {
        EXPECT_TRUE(Client.IsConnected());
    }

    TEST_F(TcpCommunicationTest, ServerSendClientReceive)
    {
        const std::size_t cBufferSize = 16;

        const std::array<uint8_t, cBufferSize> _sendBuffer{
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

        // Evaluate sending
        ssize_t _sentBytes = Server.Send(_sendBuffer);
        EXPECT_GT(_sentBytes, 0);

        // Evalute receiving
        std::array<uint8_t, cBufferSize> _receiveBuffer;
        ssize_t _receivedBytes = Client.Receive(_receiveBuffer);
        EXPECT_GT(_receivedBytes, 0);

        // Evalute data
        bool _areEqual =
            std::equal(
                std::cbegin(_sendBuffer),
                std::cend(_sendBuffer),
                std::cbegin(_receiveBuffer));
        EXPECT_TRUE(_areEqual);
    }

    TEST_F(TcpCommunicationTest, ClientSendServerReceive)
    {
        const std::size_t cBufferSize = 16;

        const std::array<uint8_t, cBufferSize> _sendBuffer{
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

        // Evaluate sending
        ssize_t _sentBytes = Client.Send(_sendBuffer);
        EXPECT_GT(_sentBytes, 0);

        // Evalute receiving
        std::array<uint8_t, cBufferSize> _receiveBuffer;
        ssize_t _receivedBytes = Server.Receive(_receiveBuffer);
        EXPECT_GT(_receivedBytes, 0);

        // Evalute data
        bool _areEqual =
            std::equal(
                std::cbegin(_sendBuffer),
                std::cend(_sendBuffer),
                std::cbegin(_receiveBuffer));
        EXPECT_TRUE(_areEqual);
    }
}