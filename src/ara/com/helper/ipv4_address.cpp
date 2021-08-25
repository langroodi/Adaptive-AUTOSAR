#include "./ipv4_address.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            Ipv4Address::Ipv4Address(
                uint8_t octet0,
                uint8_t octet1,
                uint8_t octet2,
                uint8_t octet3) noexcept
            {
                Octets = {octet0, octet1, octet2, octet3};
            }
        };
    }
}