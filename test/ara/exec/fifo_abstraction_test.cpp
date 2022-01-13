#include <gtest/gtest.h>
#include <asyncbsdsocket/fifo_receiver.h>
#include <asyncbsdsocket/fifo_sender.h>

using namespace AsyncBsdSocketLib;

namespace ara
{
    namespace com
    {
        namespace exec
        {
            namespace sd
            {
                TEST(FifoAbstractionTest, FifoReceiverConstuctor)
                {
                    const std::string cPathname = "/tmp/receivertest";
                    EXPECT_NO_THROW(FifoReceiver server(cPathname););
                }

                TEST(FifoAbstractionTest, FifoSenderConstuctor)
                {
                    const std::string cPathname = "/tmp/sendertest";
                    EXPECT_NO_THROW(FifoReceiver client(cPathname););
                }
            }
        }
    }
}