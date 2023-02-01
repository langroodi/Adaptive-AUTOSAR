#include <gtest/gtest.h>
#include <doiplib/power_mode_request.h>

namespace DoipLib
{
    TEST(PowerModeRequestTest, ValidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x03, 0x00, 0x00, 0x00, 0x00};

        PowerModeRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);
    }

    TEST(PowerModeRequestTest, InvalidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x03, 0x00, 0x00, 0x00, 0x01, 0xff};

        PowerModeRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_FALSE(_succeed);
    }
}