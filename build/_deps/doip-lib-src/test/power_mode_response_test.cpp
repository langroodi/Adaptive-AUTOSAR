#include <array>
#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/power_mode_response.h>

namespace DoipLib
{
    TEST(PowerModeResponseTest, Serialization)
    {
        const uint8_t cProtocolVersion{0x02};
        const PowerModeType cPowerMode{PowerModeType::Ready};
        const std::size_t cPayloadLength{9};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x40, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01};

        PowerModeResponse _message(cProtocolVersion, cPowerMode);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(PowerModeResponseTest, ValidDeserialization)
    {
        const PowerModeType cExpectedResult{PowerModeType::Ready};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01};

        PowerModeResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        PowerModeType _actualResult{_message.GetPowerMode()};
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(PowerModeResponseTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00};

        PowerModeResponse _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}