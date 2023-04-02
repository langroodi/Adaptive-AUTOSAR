#include <sstream>
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

            Ipv4Address::Ipv4Address(std::string ipAddress)
            {
                char _dot;
                int _octetInt0, _octetInt1, _octetInt2, _octetInt3;
                std::istringstream ipAddressStream(ipAddress.c_str());

                ipAddressStream >>
                    _octetInt0 >> _dot >>
                    _octetInt1 >> _dot >>
                    _octetInt2 >> _dot >>
                    _octetInt3 >> _dot;

                const auto cOctet0{static_cast<uint8_t>(_octetInt0)};
                const auto cOctet1{static_cast<uint8_t>(_octetInt1)};
                const auto cOctet2{static_cast<uint8_t>(_octetInt2)};
                const auto cOctet3{static_cast<uint8_t>(_octetInt3)};

                Octets = std::array<uint8_t, 4>{cOctet0, cOctet1, cOctet2, cOctet3};
            }

            std::string Ipv4Address::ToString() const
            {
                const std::string cResult{
                    std::to_string(Octets[0]) + "." +
                    std::to_string(Octets[1]) + "." +
                    std::to_string(Octets[2]) + "." +
                    std::to_string(Octets[3])};

                return cResult;
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