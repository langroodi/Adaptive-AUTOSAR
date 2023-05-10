#include <gtest/gtest.h>
#include <doiplib/vehicle_id_request.h>

namespace DoipLib
{
    TEST(VehicleIdRequestTest, ValidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

        VehicleIdRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);
    }

    TEST(VehicleIdRequestTest, ValidLongerDeserialization)
    {
        const std::vector<uint8_t> cLongerSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

        VehicleIdRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cLongerSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);
    }

    TEST(VehicleIdRequestTest, InvalidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0xff};

        VehicleIdRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_FALSE(_succeed);
    }
}