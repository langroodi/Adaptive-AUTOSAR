#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include <asyncbsdsocket/poller.h>
#include <asyncbsdsocket/tcp_client.h>
#include <asyncbsdsocket/tcp_listener.h>

namespace AsyncBsdSocketLib
{
    class PollerTest : public testing::Test
    {
    private:
        const std::string cLocalhost = "127.0.0.1";
        const uint16_t cPort = 8080;
        static const std::size_t cBufferSize = 16;
        const std::array<uint8_t, cBufferSize> mSendBuffer;

    protected:
        Poller EdgePoller;
        TcpListener Server;
        TcpClient Client;

        bool SuccessfulSetup;
        bool SuccessfulAccept;
        bool SuccessfulSend;
        bool SuccessfulReceive;

        PollerTest() : EdgePoller(),
                       Server(cLocalhost, cPort),
                       Client(cLocalhost, cPort),
                       SuccessfulSetup{false},
                       SuccessfulAccept{false},
                       SuccessfulSend{false},
                       SuccessfulReceive{false},
                       mSendBuffer{
                           0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}

        {
        }

        void SetUp()
        {
            SuccessfulSetup = Server.TrySetup();

            if (SuccessfulSetup)
            {
                SuccessfulSetup = Client.TrySetup();
            }
        }

        void TearDown()
        {
            Client.TryClose();
            Server.TryClose();
        }

    public:
        void OnAccept()
        {
            SuccessfulAccept = Server.TryAccept();

            if (SuccessfulAccept)
            {
                SuccessfulAccept = Server.TryMakeConnectionNonblock();
            }
        }

        void OnSend()
        {
            SuccessfulSend = Server.Send(mSendBuffer) > 0;
        }

        void OnReceive()
        {
            std::array<uint8_t, cBufferSize> _receiveBuffer;
            SuccessfulReceive = Client.Receive(_receiveBuffer) > 0;

            if (SuccessfulReceive)
            {
                SuccessfulReceive =
                    std::equal(
                        std::cbegin(mSendBuffer),
                        std::cend(mSendBuffer),
                        std::cbegin(_receiveBuffer));
            }
        }
    };

    TEST_F(PollerTest, ServerClientPoll)
    {
        EXPECT_TRUE(SuccessfulSetup);

        // Evaluate acceptance
        auto _onAccept = std::bind(&PollerTest::OnAccept, this);
        bool _succeed = EdgePoller.TryAddListener(&Server, _onAccept);
        EXPECT_TRUE(_succeed);
        Client.TryConnect();
        _succeed = EdgePoller.TryPoll();
        EXPECT_TRUE(_succeed);
        EXPECT_TRUE(SuccessfulAccept);

        // Evaluate sending
        auto _onSend = std::bind(&PollerTest::OnSend, this);
        _succeed = EdgePoller.TryAddSender(&Server, _onSend);
        EXPECT_TRUE(_succeed);
        _succeed = EdgePoller.TryPoll();
        EXPECT_TRUE(_succeed);
        EXPECT_TRUE(SuccessfulSend);

        // Evaluate receiving
        auto _onReceive = std::bind(&PollerTest::OnReceive, this);
        _succeed = EdgePoller.TryAddReceiver(&Client, _onReceive);
        EXPECT_TRUE(_succeed);
        _succeed = EdgePoller.TryPoll();
        EXPECT_TRUE(_succeed);
        EXPECT_TRUE(SuccessfulReceive);
    }
}