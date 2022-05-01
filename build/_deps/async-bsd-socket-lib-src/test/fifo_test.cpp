#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include <asyncbsdsocket/fifo_receiver.h>
#include <asyncbsdsocket/fifo_sender.h>

namespace AsyncBsdSocketLib
{
    class FifoTest : public testing::Test
    {
    private:
        const std::string cPathname = "/tmp/fifotest";

    protected:
        FifoReceiver Server;
        FifoSender Client;

        FifoTest() : Server(cPathname),
                     Client(cPathname)
        {
        }

        void SetUp()
        {
            bool _succeed = Server.TrySetup();

            if (_succeed)
            {
                Client.TrySetup();
            }
        }

        void TearDown()
        {
            Client.TryClose();
            Server.TryClose();
        }
    };

    TEST_F(FifoTest, DescriptorValidity)
    {
        EXPECT_GE(Server.Descriptor(), 0);
        EXPECT_GE(Client.Descriptor(), 0);
    }

    TEST_F(FifoTest, TransmissionValidity)
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