#include <gtest/gtest.h>
#include "../../../../src/ara/com/helper/ipv4_address.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            TEST(Ipv4AddressTest, Constructor)
            {
                const uint8_t cFirstOctet = 192;
                const uint8_t cSecondOctet = 168;
                const uint8_t cThirdOctet = 1;
                const uint8_t cForthOctet = 255;

                auto _ipAddress =
                    Ipv4Address(
                        cFirstOctet, cSecondOctet, cThirdOctet, cForthOctet);

                EXPECT_EQ(_ipAddress.Octets[0], cFirstOctet);
                EXPECT_EQ(_ipAddress.Octets[1], cSecondOctet);
                EXPECT_EQ(_ipAddress.Octets[2], cThirdOctet);
                EXPECT_EQ(_ipAddress.Octets[3], cForthOctet);
            }
        }
    }
}