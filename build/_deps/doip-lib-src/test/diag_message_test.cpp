#include <array>
#include <iterator>
#include <algorithm>
#include <utility>
#include <gtest/gtest.h>
#include <doiplib/diag_message.h>

namespace DoipLib
{
    TEST(DiagMessageTest, SerializationWithCopyCtor)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint8_t cSourceAddress{0x0001};
        const uint8_t cTargetAddress{0x0002};
        const std::vector<uint8_t> cUserData{0xab, 0xcd, 0xef};
        const std::size_t cPayloadLength{15};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x80, 0x01, 0x00, 0x00, 0x00, 0x07,
            0x00, 0x01, 0x00, 0x02, 0xab, 0xcd, 0xef};

        DiagMessage _message(cProtocolVersion, cSourceAddress, cTargetAddress, cUserData);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(DiagMessageTest, SerializationWithMoveCtor)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint8_t cSourceAddress{0x0001};
        const uint8_t cTargetAddress{0x0002};
        std::vector<uint8_t> _userData{0xab, 0xcd, 0xef};
        const std::size_t cPayloadLength{15};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x80, 0x01, 0x00, 0x00, 0x00, 0x07,
            0x00, 0x01, 0x00, 0x02, 0xab, 0xcd, 0xef};

        DiagMessage _message(cProtocolVersion, cSourceAddress, cTargetAddress, std::move(_userData));
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(DiagMessageTest, ValidDeserialization)
    {
        const uint8_t cSourceAddress{0x0001};
        const uint8_t cTargetAddress{0x0002};
        const std::vector<uint8_t> cUserData{0xab, 0xcd, 0xef};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x80, 0x01, 0x00, 0x00, 0x00, 0x07,
            0x00, 0x01, 0x00, 0x02, 0xab, 0xcd, 0xef};

        DiagMessage _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        EXPECT_EQ(cSourceAddress, _message.GetSourceAddress());
        EXPECT_EQ(cTargetAddress, _message.GetTargetAddress());

        std::vector<uint8_t> _actualUserData;
        _message.GetUserData(_actualUserData);
        EXPECT_EQ(cUserData, _actualUserData);
    }

    TEST(DiagMessageTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00};

        DiagMessage _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}