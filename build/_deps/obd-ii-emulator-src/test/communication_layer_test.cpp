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

        static void DummyAsyncCallback(std::vector<uint8_t> &&request);

    public:
        bool TryStart(std::vector<uint8_t> &&configuration) override
        {
            return true;
        }

        bool TrySendAsync(std::vector<uint8_t> &&data) override
        {
            return false;
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

    void CommunicationLayerTest::DummyAsyncCallback(std::vector<uint8_t> &&request)
    {
    }

    TEST_F(CommunicationLayerTest, Constructor)
    {
        EXPECT_FALSE(Callback);
        EXPECT_FALSE(AsyncCallback);
    }

    TEST_F(CommunicationLayerTest, SetCallbackMethod)
    {
        SetCallback(DummyCallback);
        EXPECT_TRUE(Callback);
        EXPECT_FALSE(AsyncCallback);
    }

    TEST_F(CommunicationLayerTest, SetAsyncCallbackMethod)
    {
        SetCallback(DummyAsyncCallback);
        EXPECT_TRUE(AsyncCallback);
        EXPECT_FALSE(Callback);
    }

    TEST_F(CommunicationLayerTest, ResetCallbackMethod)
    {
        SetCallback(DummyCallback);
        ResetCallback();
        EXPECT_FALSE(Callback);
        EXPECT_FALSE(AsyncCallback);
    }

    TEST_F(CommunicationLayerTest, ResetAsyncCallbackMethod)
    {
        SetCallback(DummyAsyncCallback);
        ResetCallback();
        EXPECT_FALSE(Callback);
        EXPECT_FALSE(AsyncCallback);
    }
}