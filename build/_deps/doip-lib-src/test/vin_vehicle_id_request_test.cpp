#include <array>
#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/vin_vehicle_id_request.h>

namespace DoipLib
{
    TEST(VinVehicleIdRequestTest, Constructor)
    {
        const uint8_t cProtocolVersion{0x02};
        const std::string cInvalidVin{"ABC"};

        EXPECT_THROW(
            VinVehicleIdRequest _message(cProtocolVersion, cInvalidVin),
            std::out_of_range);

        std::string _invalidVin{cProtocolVersion};

        EXPECT_THROW(
            VinVehicleIdRequest _messageByMovingVin(cProtocolVersion, "ABC"),
            std::out_of_range);
    }

    TEST(VinVehicleIdRequestTest, Serialization)
    {
        const uint8_t cProtocolVersion{0x02};
        const std::string cVin{"ABCDEFGHIJKLMNOPQ"};
        const std::size_t cPayloadLength{25};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x03, 0x00, 0x00, 0x00, 0x11,
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51};

        VinVehicleIdRequest _message(cProtocolVersion, cVin);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(VinVehicleIdRequestTest, ValidDeserialization)
    {
        const std::string cExpectedResult{"ABCDEFGHIJKLMNOPQ"};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x03, 0x00, 0x00, 0x00, 0x11,
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51};

        VinVehicleIdRequest _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        std::string _actualResult{_message.GetVin()};
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(VinVehicleIdRequestTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00};

        VinVehicleIdRequest _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}