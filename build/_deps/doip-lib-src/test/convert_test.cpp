#include <iterator>
#include <gtest/gtest.h>
#include <doiplib/convert.h>
#include <doiplib/payload_type.h>

namespace DoipLib
{
    TEST(ConvertTest, ShortToByteVector)
    {
        const uint16_t cNumber{0x1234};
        const std::array<uint8_t, sizeof(uint16_t)> cExpectedResult{0x12, 0x34};

        std::vector<uint8_t> _actualResult;
        Convert::ToByteVector<uint16_t>(cNumber, _actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(ConvertTest, IntegerToByteVector)
    {
        const uint32_t cNumber{0x12345678};
        const std::array<uint32_t, sizeof(uint32_t)> cExpectedResult{0x12, 0x34, 0x56, 0x78};

        std::vector<uint8_t> _actualResult;
        Convert::ToByteVector<uint32_t>(cNumber, _actualResult);

        bool _areEqual =
            std::equal(
                std::cbegin(cExpectedResult),
                std::cend(cExpectedResult),
                std::cbegin(_actualResult));
        EXPECT_TRUE(_areEqual);
    }

    TEST(ConvertTest, ByteVectorToShort)
    {
        const uint16_t cExpectedResult{0x1234};
        std::vector<uint8_t> _byteVector{0x12, 0x34};
        std::size_t _offset{0};

        auto _actualResult{Convert::ToUnsignedInteger<uint16_t>(_byteVector, _offset)};

        EXPECT_EQ(cExpectedResult, _actualResult);
        EXPECT_EQ(sizeof(uint16_t), _offset);
    }

    TEST(ConvertTest, ByteVectorToInteger)
    {
        const uint32_t cExpectedResult{0x12345678};
        std::vector<uint8_t> _byteVector{0x12, 0x34, 0x56, 0x78};
        std::size_t _offset{0};

        auto _actualResult{Convert::ToUnsignedInteger<uint32_t>(_byteVector, _offset)};

        EXPECT_EQ(cExpectedResult, _actualResult);
        EXPECT_EQ(sizeof(uint32_t), _offset);
    }

    TEST(ConvertTest, ByteVectorToEnum)
    {
        const PayloadType cExpectedResult{PayloadType::DiagMessage};
        std::vector<uint8_t> _byteVector{0x80, 0x1};
        std::size_t _offset{0};

        auto _actualResult{Convert::ToEnum<PayloadType>(_byteVector, _offset)};

        EXPECT_EQ(cExpectedResult, _actualResult);
        EXPECT_EQ(sizeof(PayloadType), _offset);
    }

    TEST(ConvertTest, ByteVectorToByteArray)
    {
        const std::size_t cArraySize{6};
        const std::array<uint8_t, cArraySize> cExpectedResult{0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
        std::vector<uint8_t> _byteVector{0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
        std::size_t _offset{0};

        auto _actualResult{Convert::ToByteArray<cArraySize>(_byteVector, _offset)};

        EXPECT_EQ(cExpectedResult, _actualResult);
        EXPECT_EQ(cArraySize, _offset);
    }

    TEST(ConvertTest, ByteVectorToString)
    {
        const std::size_t cArraySize{6};
        const std::string cExpectedResult{"ABCDEF"};
        std::vector<uint8_t> _byteVector{0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
        std::size_t _offset{0};

        auto _actualResult{Convert::ToString<cArraySize>(_byteVector, _offset)};

        EXPECT_EQ(cExpectedResult, _actualResult);
        EXPECT_EQ(cArraySize, _offset);
    }
}