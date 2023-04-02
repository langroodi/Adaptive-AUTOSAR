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

                /// @brief Constructor
                /// @param ipAddress IPv4 address string
                Ipv4Address(std::string ipAddress);

                /// @brief Convert the IP address to string
                /// @return IP address in string format
                std::string ToString() const;

                ~Ipv4Address() noexcept = default;

                /// @brief Inject an IP address into a byte vector
                /// @param vector Byte vector
                /// @param ipAddress IP address to be injected
                static void Inject(
                    std::vector<uint8_t> &vector,
                    Ipv4Address ipAddress);

                /// @brief Extract an IPv4 address from a byte vector
                /// @param vector Byte vector
                /// @param offset Extract offset at the vector
                /// @returns Extracted IPv4 address
                static Ipv4Address Extract(
                    const std::vector<uint8_t> &vector,
                    std::size_t &offset);
            };

            /// @brief Ipv4Address equality operator override
            /// @param address1 First address to compare
            /// @param address2 Second address to compare
            /// @returns True if both addresses are equal; otherwise false
            inline bool operator==(Ipv4Address address1, Ipv4Address address2)
            {
                bool _result =
                    (address1.Octets[0] == address2.Octets[0]) &&
                    (address1.Octets[1] == address2.Octets[1]) &&
                    (address1.Octets[2] == address2.Octets[2]) &&
                    (address1.Octets[3] == address2.Octets[3]);

                return _result;
            }

            /// @brief Ipv4Address inequality operator override
            /// @param address1 First address to compare
            /// @param address2 Second address to compare
            /// @returns True if both addresses are not equal; otherwise false
            inline bool operator!=(Ipv4Address address1, Ipv4Address address2)
            {
                bool _result = !(address1 == address2);

                return _result;
            }
        }
    }
}

#endif