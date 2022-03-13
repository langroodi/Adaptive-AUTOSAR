#include <gtest/gtest.h>
#include <obdemulator/can_driver.h>

using namespace ObdEmulator;

namespace ara
{
    namespace diag
    {
        TEST(ObdCommunicationTest, CanDriverConstructor)
        {
            const bool cSupportExtended{false};
            const CanBusSpeed cSpeed{CanBusSpeed::Speed250kbps};

            EXPECT_NO_THROW(CanDriver canDriver(cSpeed, cSupportExtended););
        }
    }
}