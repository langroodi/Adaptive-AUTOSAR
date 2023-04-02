#include <gtest/gtest.h>
#include <obdemulator/obd_service.h>

namespace ObdEmulator
{
    class ObdServiceTest : public testing::Test, public ObdService
    {
    protected:
        static const uint8_t cService{0x01};

        ObdServiceTest() : ObdService(cService)
        {
        }

    public:
        bool TryGetResponse(
            const std::vector<uint8_t> &query,
            std::vector<uint8_t> &response) const override
        {
            return false;
        }

        bool TryGetResponseAsync(
            const std::vector<uint8_t> &pid,
            std::function<void(std::vector<uint8_t> &&)> &&callback) override
        {
            return false;
        }
    };

    const uint8_t ObdServiceTest::cService;

    TEST_F(ObdServiceTest, GetServiceMethod)
    {
        EXPECT_EQ(GetService(), cService);
    }
}