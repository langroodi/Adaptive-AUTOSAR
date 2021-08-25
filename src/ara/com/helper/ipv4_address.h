#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <array>
#include <stdint.h>

namespace ara
{
    namespace com
    {
        namespace helper
        {
            struct Ipv4Address
            {
                /// @brief IPv4 address octets
                std::array<uint8_t, 4> Octets;

                Ipv4Address() = delete;

                /// @brief Constructor
                /// @param octet0 IPv4 first octet
                /// @param octet1 IPv4 second octet
                /// @param octet2 IPv4 thrid octet
                /// @param octet3 IPv4 forth octet
                Ipv4Address(
                    uint8_t octet0,
                    uint8_t octet1,
                    uint8_t octet2,
                    uint8_t octet3) noexcept;

                ~Ipv4Address() noexcept = default;
            };
        }
    }
}

#endif