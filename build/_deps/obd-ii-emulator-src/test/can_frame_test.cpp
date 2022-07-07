#include <gtest/gtest.h>
#include <obdemulator/can_frame.h>

namespace ObdEmulator
{
    class CanFrameTest : public testing::Test
    {
    protected:
        const uint32_t cId{1};
        const bool cExtended{true};
        const bool cRemote{false};
        const std::array<uint8_t, CanFrame::cDataLengthMax> cDataArray;

        CanFrame Frame;

        CanFrameTest() : Frame(cId, cExtended, cRemote, {0, 1, 2, 3, 4, 5, 6, 7}),
                         cDataArray{0, 1, 2, 3, 4, 5, 6, 7}
        {
        }
    };

    TEST_F(CanFrameTest, IdProperty)
    {
        EXPECT_EQ(Frame.GetId(), cId);
    }

    TEST_F(CanFrameTest, ExtendedProperty)
    {
        EXPECT_EQ(Frame.IsExtended(), cExtended);
    }

    TEST_F(CanFrameTest, RemoteProperty)
    {
        EXPECT_EQ(Frame.IsRemote(), cRemote);
    }

    TEST_F(CanFrameTest, DataLengthProperty)
    {
        EXPECT_EQ(Frame.GetDataLength(), cDataArray.size());
    }

    TEST_F(CanFrameTest, DataProperty)
    {
        auto _data = Frame.GetData();
        EXPECT_TRUE(std::equal(_data.begin(), _data.end(), cDataArray.begin()));
    }
}