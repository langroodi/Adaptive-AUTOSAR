#include <array>
#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/message.h>

namespace DoipLib
{
    class MessageTest : public Message, public testing::Test
    {
    private:
        static const uint8_t cProtocolVersion{0x02};
        static const PayloadType cPayloadType{PayloadType::VehicleIdRequest};

    protected:
        void GetPayload(std::vector<uint8_t> &payload) const override
        {
        }

        virtual bool TrySetPayload(
            const std::vector<uint8_t> &payload,
            uint32_t payloadLength) override
        {
            return true;
        }

    public:
        MessageTest() noexcept : Message(cProtocolVersion, cPayloadType)
        {
        }
    };

    const uint8_t MessageTest::cProtocolVersion;
    const PayloadType MessageTest::cPayloadType;

    TEST_F(MessageTest, Serialization)
    {
        const std::size_t cPayloadLength{8};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

        std::vector<uint8_t> _actualResult;
        Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST_F(MessageTest, ValidDeserialization)
    {
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

        GenericNackType _nackCode;
        bool _succeed{TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);
    }

    TEST_F(MessageTest, ValidLongMessageDeserialization)
    {
        const std::vector<uint8_t> cLongerSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff};

        GenericNackType _nackCode;
        bool _succeed{TryDeserialize(cLongerSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);
    }

    TEST_F(MessageTest, InvalidHeaderDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00};

        GenericNackType _actualResult;
        bool _succeed{TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }

    TEST_F(MessageTest, InvalidVersionDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidProtocolVersion};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

        GenericNackType _actualResult;
        bool _succeed{TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }

    TEST_F(MessageTest, InvalidTypeDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::UnsupportedPayloadType};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        GenericNackType _actualResult;
        bool _succeed{TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }

    TEST_F(MessageTest, InvalidLengthDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01};

        GenericNackType _actualResult;
        bool _succeed{TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }

    TEST_F(MessageTest, ValidPayloadTypeExtraction)
    {
        const PayloadType cExpectedResult{PayloadType::VehicleIdRequest};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

        PayloadType _actualResult;
        bool _succeed{
            Message::TryExtractPayloadType(cSerializedMessage, _actualResult)};

        EXPECT_TRUE(_succeed);
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST_F(MessageTest, InvalidPayloadTypeExtraction)
    {
        const PayloadType cExpectedResult{PayloadType::VehicleIdRequest};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x01};

        PayloadType _actualResult;
        bool _succeed{
            Message::TryExtractPayloadType(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
    }
}