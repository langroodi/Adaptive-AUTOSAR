#include <gtest/gtest.h>
#include "../../../../../src/ara/com/someip/someip_message.h"
#include "../../../../../src/ara/com/someip/sd/someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                TEST(SomeIpSdMessageTest, Constructor)
                {
                    const uint16_t cSessionId = 1;
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Notification;
                    const SomeIpReturnCode cReturnCode = SomeIpReturnCode::eOK;

                    SomeIpSdMessage _message;

                    EXPECT_EQ(_message.SessionId(), cSessionId);
                    EXPECT_EQ(_message.MessageType(), cMessageType);
                    EXPECT_EQ(_message.ReturnCode(), cReturnCode);
                }
            }
        }
    }
}