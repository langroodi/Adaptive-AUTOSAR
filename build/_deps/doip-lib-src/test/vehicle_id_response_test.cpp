#include <iterator>
#include <algorithm>
#include <gtest/gtest.h>
#include <doiplib/vehicle_id_response.h>

namespace DoipLib
{
    TEST(VehicleIdResponeTest, Constructor)
    {
        const uint8_t cProtocolVersion{0x02};
        const std::string cInvalidVin{"ABC"};
        const uint16_t cLogicalAddress{0x0001};
        const std::size_t cIdSize{6};
        const std::array<uint8_t, cIdSize> cId{0x01, 0x23, 0x45, 0x67, 0x89, 0xab};
        const uint8_t cFurtherAction{0x00};
        const uint8_t cVinGinStatus{0x10};

        EXPECT_THROW(
            VehicleIdResponse _messageWithoutSync(
                cProtocolVersion,
                cInvalidVin,
                cLogicalAddress,
                cId,
                cId,
                cFurtherAction),
            std::out_of_range);

        EXPECT_THROW(
            VehicleIdResponse _messageUsingSync(
                cProtocolVersion,
                cInvalidVin,
                cLogicalAddress,
                cId,
                cId,
                cFurtherAction,
                cVinGinStatus),
            std::out_of_range);

        EXPECT_THROW(
            VehicleIdResponse _messageWithoutSyncByVinMove(
                cProtocolVersion,
                "ABC",
                cLogicalAddress,
                cId,
                cId,
                cFurtherAction),
            std::out_of_range);

        EXPECT_THROW(
            VehicleIdResponse _messageUsingSyncByVinMove(
                cProtocolVersion,
                "ABC",
                cLogicalAddress,
                cId,
                cId,
                cFurtherAction,
                cVinGinStatus),
            std::out_of_range);
    }

    TEST(VehicleIdResponeTest, SerializationWithoutSync)
    {
        const uint8_t cProtocolVersion{0x02};
        const std::string cVin{"ABCDEFGHIJKLMNOPQ"};
        const uint16_t cLogicalAddress{0x0001};
        const std::size_t cIdSize{6};
        const std::array<uint8_t, cIdSize> cEid{0x01, 0x23, 0x45, 0x67, 0x89, 0xab};
        const std::array<uint8_t, cIdSize> cGid{0x10, 0x32, 0x54, 0x76, 0x98, 0xba};
        const uint8_t cFurtherAction{0x00};
        const std::size_t cPayloadLength{40};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20,
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
            0x00, 0x01,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab,
            0x10, 0x32, 0x54, 0x76, 0x98, 0xba,
            0x00};

        VehicleIdResponse _message(
            cProtocolVersion, cVin, cLogicalAddress, cEid, cGid, cFurtherAction);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(VehicleIdResponeTest, SerializationUsingSync)
    {
        const uint8_t cProtocolVersion{0x02};
        const std::string cVin{"ABCDEFGHIJKLMNOPQ"};
        const uint16_t cLogicalAddress{0x0001};
        const std::size_t cIdSize{6};
        const std::array<uint8_t, cIdSize> cEid{0x01, 0x23, 0x45, 0x67, 0x89, 0xab};
        const std::array<uint8_t, cIdSize> cGid{0x10, 0x32, 0x54, 0x76, 0x98, 0xba};
        const uint8_t cFurtherAction{0x00};
        const uint8_t cVinGinStatus{0x10};
        const std::size_t cPayloadLength{41};
        const std::array<uint8_t, cPayloadLength> cExpectedResult{
            0x02, 0xfd, 0x00, 0x04, 0x00, 0x00, 0x00, 0x21,
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
            0x00, 0x01,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab,
            0x10, 0x32, 0x54, 0x76, 0x98, 0xba,
            0x00,
            0x10};

        VehicleIdResponse _message(
            cProtocolVersion, cVin, cLogicalAddress, cEid, cGid, cFurtherAction, cVinGinStatus);
        std::vector<uint8_t> _actualResult;
        _message.Serialize(_actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(VehicleIdResponeTest, DeserializationWithoutSync)
    {
        const std::string cVin{"ABCDEFGHIJKLMNOPQ"};
        const uint16_t cLogicalAddress{0x0001};
        const std::size_t cIdSize{6};
        const std::array<uint8_t, cIdSize> cEid{0x01, 0x23, 0x45, 0x67, 0x89, 0xab};
        const std::array<uint8_t, cIdSize> cGid{0x10, 0x32, 0x54, 0x76, 0x98, 0xba};
        const uint8_t cFurtherAction{0x00};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20,
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
            0x00, 0x01,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab,
            0x10, 0x32, 0x54, 0x76, 0x98, 0xba,
            0x00};

        VehicleIdResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        EXPECT_EQ(cVin, _message.GetVin());
        EXPECT_EQ(cLogicalAddress, _message.GetLogicalAddress());
        EXPECT_EQ(cEid, _message.GetEid());
        EXPECT_EQ(cGid, _message.GetGid());
        EXPECT_EQ(cFurtherAction, _message.GetFurtherAction());
    }

    TEST(VehicleIdResponeTest, DeserializationUsingSync)
    {
        const uint8_t cExpectedResult{0x10};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x04, 0x00, 0x00, 0x00, 0x21,
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
            0x00, 0x01,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab,
            0x10, 0x32, 0x54, 0x76, 0x98, 0xba,
            0x00,
            0x10};

        VehicleIdResponse _message;
        GenericNackType _nackCode;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _nackCode)};
        EXPECT_TRUE(_succeed);

        uint8_t _actualResult;
        _succeed = {_message.TryGetVinGinStatus(_actualResult)};
        EXPECT_TRUE(_succeed);
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(VehicleIdResponeTest, InvalidDeserialization)
    {
        const GenericNackType cExceptedResult{GenericNackType::InvalidPayloadLength};
        const std::vector<uint8_t> cSerializedMessage{
            0x02, 0xfd, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};

        VehicleIdResponse _message;
        GenericNackType _actualResult;
        bool _succeed{_message.TryDeserialize(cSerializedMessage, _actualResult)};

        EXPECT_FALSE(_succeed);
        EXPECT_EQ(cExceptedResult, _actualResult);
    }
}