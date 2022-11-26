#include <gtest/gtest.h>
#include <obdemulator/can_driver.h>

namespace ObdEmulator
{
    class CanDriverTest : public testing::Test
    {
    protected:
        const CanBusSpeed cSpeed{CanBusSpeed::Speed5kbps};
        const bool cSupportExtended{true};

        CanDriver Driver;

        CanDriverTest() : Driver(cSpeed, cSupportExtended)
        {
        }
    };

    TEST_F(CanDriverTest, GetConfigurationMethod)
    {
        const std::array<uint8_t, CanDriver::cFixedFrameSize> cExpectedResult{
            0xaa, 0x55, 0x12,
            0x0c,
            0x02,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00,
            0x01, 0x00, 0x00, 0x00, 0x00,
            0x21};

        auto _actualResult{Driver.GetConfiguration()};

        EXPECT_TRUE(
            std::equal(
                cExpectedResult.begin(),
                cExpectedResult.end(),
                _actualResult.begin()));
    }

    TEST_F(CanDriverTest, StandardSerializeMethod)
    {
        const size_t cPacketSize{13};
        const uint32_t cId{0x0123};
        const bool cExtended{false};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const CanFrame cFrame(cId, cExtended, cRtr, cData);

        const std::array<uint8_t, cPacketSize> cExpectedResult{
            0xaa, 0xc8,
            0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        auto _actualResult{Driver.Serialize(cFrame)};

        EXPECT_TRUE(
            std::equal(
                cExpectedResult.begin(),
                cExpectedResult.end(),
                _actualResult.begin()));
    }

    TEST_F(CanDriverTest, ExtendedSerializeMethod)
    {
        const size_t cPacketSize{15};
        const uint32_t cId{0x01234567};
        const bool cExtended{true};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const CanFrame cFrame(cId, cExtended, cRtr, cData);

        const std::array<uint8_t, cPacketSize> cExpectedResult{
            0xaa, 0xe8,
            0x67, 0x45, 0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        auto _actualResult{Driver.Serialize(cFrame)};

        EXPECT_TRUE(
            std::equal(
                cExpectedResult.begin(),
                cExpectedResult.end(),
                _actualResult.begin()));
    }

    TEST_F(CanDriverTest, StandardDeserializeMethod)
    {
        const uint32_t cId{0x0123};
        const bool cExtended{false};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const std::initializer_list<uint8_t> cPacket{
            0xaa, 0xc8,
            0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        CanFrame _frame{Driver.Deserialize(cPacket)};

        EXPECT_EQ(_frame.GetId(), cId);
        EXPECT_EQ(_frame.IsExtended(), cExtended);
        EXPECT_EQ(_frame.IsRemote(), cRtr);
        EXPECT_EQ(_frame.GetDataLength(), cData.size());
        EXPECT_TRUE(
            std::equal(
                cData.begin(),
                cData.end(),
                _frame.GetData().begin()));
    }

    TEST_F(CanDriverTest, ExtendedDeserializeMethod)
    {
        const uint32_t cId{0x01234567};
        const bool cExtended{true};
        const bool cRtr{false};
        const std::initializer_list<uint8_t> cData{
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

        const std::initializer_list<uint8_t> cPacket{
            0xaa, 0xe8,
            0x67, 0x45, 0x23, 0x01,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x55};

        CanFrame _frame{Driver.Deserialize(cPacket)};

        EXPECT_EQ(_frame.GetId(), cId);
        EXPECT_EQ(_frame.IsExtended(), cExtended);
        EXPECT_EQ(_frame.IsRemote(), cRtr);
        EXPECT_EQ(_frame.GetDataLength(), cData.size());
        EXPECT_TRUE(
            std::equal(
                cData.begin(),
                cData.end(),
                _frame.GetData().begin()));
    }
}