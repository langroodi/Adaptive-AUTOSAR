#include <gtest/gtest.h>
#include <doiplib/alive_check_request.h>

namespace DoipLib
{
    TEST(AliveCheckRequestTest, ValidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00};

        AliveCheckRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);
    }

    TEST(AliveCheckRequestTest, InvalidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0xff};

        AliveCheckRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_FALSE(_succeed);
    }
}