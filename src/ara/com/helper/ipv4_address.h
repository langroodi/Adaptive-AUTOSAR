#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <array>
#include <vector>
#include <stdint.h>

namespace ara
{
    namespace com
    {
        namespace helper
        {
            /// @brief IPv4 address wrapper helper
            struct Ipv4Address
            {
                /// @brief IPv4 address octets
                std::array<uint8_t, 4> Octets;

                Ipv4Address() = delete;

                /// @brief Constructor
                /// @param octet0 IPv4 first octet
                /// @param octet1 IPv4 second octet
                /// @param octet2 IPv4 third octet
                /// @param octet3 IPv4 forth octet
                Ipv4Address(
                    uint8_t octet0,
                    uint8_t octet1,
                    uint8_t octet2,
                    uint8_t octet3) noexcept;

                ~Ipv4Address() noexcept = default;

                /// @brief Inject an IP address into a byte vector
                /// @param vector Byte vector
                /// @param ipAddress IP address to be injected
                static void Inject(
                    std::vector<uint8_t> &vector,
                    Ipv4Address ipAddress);
            };
        }
    }
}

#endif