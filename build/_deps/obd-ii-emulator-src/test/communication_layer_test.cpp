#include <gtest/gtest.h>
#include <obdemulator/communication_layer.h>

namespace ObdEmulator
{
    class CommunicationLayerTest : public testing::Test, public CommunicationLayer
    {
    protected:
        static bool DummyCallback(
            std::vector<uint8_t> &&request,
            std::vector<uint8_t> &response);

    public:
        bool TryStart(std::vector<uint8_t> &&configuration) override
        {
            return true;
        }

        bool TryStop() override
        {
            return true;
        }
    };

    bool CommunicationLayerTest::DummyCallback(
        std::vector<uint8_t> &&request,
        std::vector<uint8_t> &response)
    {
        return false;
    }

    TEST_F(CommunicationLayerTest, Constructor)
    {
        EXPECT_FALSE(Callback);
    }

    TEST_F(CommunicationLayerTest, SetCallbackMethod)
    {
        SetCallback(DummyCallback);
        EXPECT_TRUE(Callback);
    }

    TEST_F(CommunicationLayerTest, ResetCallbackMethod)
    {
        SetCallback(DummyCallback);
        ResetCallback();
        EXPECT_FALSE(Callback);
    }
}