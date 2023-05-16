#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/routing_activation_response.h>

namespace DoipLib
{
    TEST(RoutingActivationResponseTest, SerializationWithoutOemData)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint16_t cTesterLogicalAddress{0x0001};
        const uint16_t cEntityLogicalAddress{0x0002};
        const RoutingActivationResponseType cResponseCode{RoutingActivationResponseType::Successful};
        const std::size_t cPayloadLength{17};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x06, 0x00, 0x00, 0x00, 0x09,
            0x00, 0x01, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00};

        RoutingActivationResponse _message(
            cProtocolVersion, cTesterLogicalAddress, cEntityLogicalAddress, cResponseCode);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(RoutingActivationResponseTest, SerializationUsingOemData)
    {
        const uint8_t cProtocolVersion{0x02};
        const uint16_t cTesterLogicalAddress{0x0001};
        const uint16_t cEntityLogicalAddress{0x0002};
        const RoutingActivationResponseType cResponseCode{RoutingActivationResponseType::Successful};
        const uint32_t cOemSpecificData{0x12345678};
        const std::size_t cPayloadLength{21};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0d,
            0x00, 0x01, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78};

        RoutingActivationResponse _message(
            cProtocolVersion, cTesterLogicalAddress, cEntityLogicalAddress, cResponseCode, cOemSpecificData);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(RoutingActivationResponseTest, DeserializationWithoutOemData)
    {
        const uint16_t cTesterLogicalAddress{0x0001};
        const uint16_t cEntityLogicalAddress{0x0002};
        const RoutingActivationResponseType cResponseCode{RoutingActivationResponseType::Successful};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x06, 0x00, 0x00, 0x00, 0x09,
            0x00, 0x01, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00};

        RoutingActivationResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        EXPECT_EQ(cTesterLogicalAddress, _message.GetTesterLogicalAddress());
        EXPECT_EQ(cEntityLogicalAddress, _message.GetEntityLogicalAddress());
        EXPECT_EQ(cResponseCode, _message.GetResponseCode());
    }

    TEST(RoutingActivationResponseTest, DeserializationUsingOemData)
    {
        const uint32_t cExpectedResult{0x12345678};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0d,
            0x00, 0x01, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78};

        RoutingActivationResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        uint32_t _actualResult;
        _succeed = {_message.TryGetOemSpecificData(_actualResult)};
        EXPECT_TRUE(_succeed);
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(RoutingActivationResponseTest, InvalidDeserialization)
    {
                const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00};

        RoutingActivationResponse _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}