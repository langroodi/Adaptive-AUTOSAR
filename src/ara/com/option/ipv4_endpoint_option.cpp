#include "./ipv4_endpoint_option.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            Ipv4EndpointOption::Ipv4EndpointOption(
                OptionType type,
                bool discardable,
                helper::Ipv4Address ipAddress,
                Layer4ProtocolType protocol,
                uint16_t port) noexcept : Option(type, discardable),
                                          mIpAddress{ipAddress},
                                          mL4Proto{protocol},
                                          mPort{port}
            {
            }

            uint16_t Ipv4EndpointOption::Length()
            {
                const uint8_t cOptionLength = 9;
                return cOptionLength;
            }

            helper::Ipv4Address Ipv4EndpointOption::IpAddress()
            {
                return mIpAddress;
            }

            Layer4ProtocolType Ipv4EndpointOption::L4Proto()
            {
                return mL4Proto;
            }

            uint16_t Ipv4EndpointOption::Port()
            {
                return mPort;
            }

            std::vector<uint8_t> Ipv4EndpointOption::Payload()
            {
                auto _result = Option::Payload();

                helper::Ipv4Address::Inject(_result, mIpAddress);

                const uint8_t cReservedByte = 0x00;
                _result.push_back(cReservedByte);

                uint8_t _protocolByte = static_cast<uint8_t>(mL4Proto);
                _result.push_back(_protocolByte);

                helper::Inject(_result, mPort);

                return _result;
            }

            Ipv4EndpointOption Ipv4EndpointOption::CreateUnitcastEndpoint(
                bool discardable,
                helper::Ipv4Address ipAddress,
                Layer4ProtocolType protocol,
                uint16_t port) noexcept
            {
                Ipv4EndpointOption _result(
                    OptionType::IPv4Endpoint,
                    discardable,
                    ipAddress,
                    protocol,
                    port);

                return _result;
            }

            Ipv4EndpointOption Ipv4EndpointOption::CreateMulticastEndpoint(
                bool discardable,
                helper::Ipv4Address ipAddress,
                uint16_t port)
            {
                const uint8_t cMulticastOctetMin = 244;
                const uint8_t cMulticastOctetMax = 239;

                uint8_t _firstOctet = ipAddress.Octets[0];
                if ((_firstOctet < cMulticastOctetMin) &&
                    (_firstOctet > cMulticastOctetMax))
                {
                    throw std::invalid_argument("IP address is out of range.");
                }

                Ipv4EndpointOption _result(
                    OptionType::IPv4Multicast,
                    discardable,
                    ipAddress,
                    Layer4ProtocolType::Udp,
                    port);

                return _result;
            }

            Ipv4EndpointOption Ipv4EndpointOption::CreateSdEndpoint(
                bool discardable,
                helper::Ipv4Address ipAddress,
                Layer4ProtocolType protocol,
                uint16_t port) noexcept
            {
                Ipv4EndpointOption _result(
                    OptionType::IPv4SdEndpoint,
                    discardable,
                    ipAddress,
                    protocol,
                    port);

                return _result;
            }
        }
    }
}