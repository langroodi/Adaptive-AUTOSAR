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
                uint8_t octet3) noexcept : Octets{octet0, octet1, octet2, octet3}
            {
            }

            void Ipv4Address::Inject(
                std::vector<uint8_t> &vector,
                Ipv4Address ipAddress)
            {
                vector.insert(
                    vector.end(),
                    ipAddress.Octets.begin(),
                    ipAddress.Octets.end());
            }

            Ipv4Address Ipv4Address::Extract(
                const std::vector<uint8_t> &vector,
                std::size_t &offset)
            {
                uint8_t _octet0 = vector.at(offset++);
                uint8_t _octet1 = vector.at(offset++);
                uint8_t _octet2 = vector.at(offset++);
                uint8_t _octet3 = vector.at(offset++);
                Ipv4Address _result(_octet0, _octet1, _octet2, _octet3);

                return _result;
            }
        };
    }
}