#include <array>
#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/alive_check_response.h>

namespace DoipLib
{
    TEST(AliveCheckResponseTest, Serialization)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint16_t cSourceAddress{0x0001};
        const std::size_t cPayloadLength{10};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01};

        AliveCheckResponse _message(cProtocolVersion, cSourceAddress);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(AliveCheckResponseTest, ValidDeserialization)
    {
        const uint16_t cExpectedResult{0x0001};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01};

        AliveCheckResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        uint16_t _actualResult{_message.GetSourceAddress()};
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(AliveCheckResponseTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00};

        AliveCheckResponse _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}