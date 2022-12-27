#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/entity_status_response.h>

namespace DoipLib
{
    TEST(EntityStatusResponseTest, SerializationWithoutMaxSize)
    {
        const uint8_t cProtocolVersion{0x02};
        const NodeType cEntityType{NodeType::DoipNode};
        const uint8_t cMaxOpenSockets{0x0f};
        const uint8_t cCurrentlyOpenSocket{0x0a};
        const std::size_t cPayloadLength{11};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x40, 0x02, 0x00, 0x00, 0x00, 0x03,
            0x01, 0x0f, 0x0a};

        EntityStatusResponse _message(
            cProtocolVersion, cEntityType, cMaxOpenSockets, cCurrentlyOpenSocket);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(EntityStatusResponseTest, SerializationUsingMaxSize)
    {
        const uint8_t cProtocolVersion{0x02};
        const NodeType cEntityType{NodeType::DoipNode};
        const uint8_t cMaxOpenSockets{0x0f};
        const uint8_t cCurrentlyOpenSocket{0x0a};
        const uint32_t cMaxDataSize{0x12345678};
        const std::size_t cPayloadLength{15};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x40, 0x02, 0x00, 0x00, 0x00, 0x07,
            0x01, 0x0f, 0x0a, 0x12, 0x34, 0x56, 0x78};

        EntityStatusResponse _message(
            cProtocolVersion, cEntityType, cMaxOpenSockets, cCurrentlyOpenSocket, cMaxDataSize);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(EntityStatusResponseTest, DeserializationWithoutMaxSize)
    {
        const NodeType cEntityType{NodeType::DoipNode};
        const uint8_t cMaxOpenSockets{0x0f};
        const uint8_t cCurrentlyOpenSocket{0x0a};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x02, 0x00, 0x00, 0x00, 0x03,
            0x01, 0x0f, 0x0a};

        EntityStatusResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        EXPECT_EQ(cEntityType, _message.GetEntityType());
        EXPECT_EQ(cMaxOpenSockets, _message.GetMaxOpenSockets());
        EXPECT_EQ(cCurrentlyOpenSocket, _message.GetCurrentlyOpenSocket());
    }

    TEST(EntityStatusResponseTest, DeserializationUsingMaxSize)
    {
        const uint32_t cExpectedResult{0x12345678};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x02, 0x00, 0x00, 0x00, 0x07,
            0x01, 0x0f, 0x0a, 0x12, 0x34, 0x56, 0x78};

        EntityStatusResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        uint32_t _actualResult;
        _succeed = {_message.TryGetMaxDataSize(_actualResult)};
        EXPECT_TRUE(_succeed);
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(EntityStatusResponseTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00};

        EntityStatusResponse _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}