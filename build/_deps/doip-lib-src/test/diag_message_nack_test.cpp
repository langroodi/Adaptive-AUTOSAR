#include <iterator>
#include <algorithm>
#include <utility>
#include <gtest/gtest.h>
#include <doiplib/diag_message_nack.h>

namespace DoipLib
{
    TEST(DiagMessageNackTest, SerializationWithoutPreviousMessage)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint16_t cSourceAddress{0x0001};
        const uint16_t cTargetAddress{0x0002};
        const DiagNackType cNackCode{DiagNackType::UnknownNetwork};
        const std::size_t cPayloadLength{13};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x80, 0x03, 0x00, 0x00, 0x00, 0x05,
            0x00, 0x01, 0x00, 0x02, 0x07};

        DiagMessageNack _message(
            cProtocolVersion, cSourceAddress, cTargetAddress, cNackCode);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(DiagMessageNackTest, SerializationCopyingPreviousMessage)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint16_t cSourceAddress{0x0001};
        const uint16_t cTargetAddress{0x0002};
        const DiagNackType cNackCode{DiagNackType::UnknownNetwork};
        const std::vector<uint8_t> cPreviousMessage{0xab, 0xcd, 0xef};
        const std::size_t cPayloadLength{16};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x80, 0x03, 0x00, 0x00, 0x00, 0x08,
            0x00, 0x01, 0x00, 0x02, 0x07, 0xab, 0xcd, 0xef};

        DiagMessageNack _message(
            cProtocolVersion, cSourceAddress, cTargetAddress, cNackCode, cPreviousMessage);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(DiagMessageNackTest, SerializationMovingPreviousMessage)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint16_t cSourceAddress{0x0001};
        const uint16_t cTargetAddress{0x0002};
        const DiagNackType cNackCode{DiagNackType::UnknownNetwork};
        std::vector<uint8_t> _previousMessage{0xab, 0xcd, 0xef};
        const std::size_t cPayloadLength{16};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x80, 0x03, 0x00, 0x00, 0x00, 0x08,
            0x00, 0x01, 0x00, 0x02, 0x07, 0xab, 0xcd, 0xef};

        DiagMessageNack _message(
            cProtocolVersion, cSourceAddress, cTargetAddress, cNackCode, std::move(_previousMessage));
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(DiagMessageNackTest, ValidDeserializationWithoutPreviousMessage)
    {
        const uint16_t cSourceAddress{0x0001};
        const uint16_t cTargetAddress{0x0002};
        const DiagNackType cNackCode{DiagNackType::UnknownNetwork};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x80, 0x03, 0x00, 0x00, 0x00, 0x05,
            0x00, 0x01, 0x00, 0x02, 0x07};

        DiagMessageNack _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        EXPECT_EQ(cSourceAddress, _message.GetSourceAddress());
        EXPECT_EQ(cTargetAddress, _message.GetTargetAddress());
        EXPECT_EQ(cNackCode, _message.GetNackCode());
    }

    TEST(DiagMessageNackTest, ValidDeserializationUsingPreviousMessage)
    {
        const std::vector<uint8_t> cExpectedResult{0xab, 0xcd, 0xef};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x80, 0x03, 0x00, 0x00, 0x00, 0x08,
            0x00, 0x01, 0x00, 0x02, 0x07, 0xab, 0xcd, 0xef};

        DiagMessageNack _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        std::vector<uint8_t> _actualResult;
        _succeed = {_message.TryGetPreviousMessage(_actualResult)};
        EXPECT_TRUE(_succeed);
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(DiagMessageNackTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00};

        DiagMessageNack _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}