#include "./ipv4_endpoint_option.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            uint16_t Ipv4EndpointOption::Length() const noexcept
            {
                const uint8_t cOptionLength = 9;
                return cOptionLength;
            }

            helper::Ipv4Address Ipv4EndpointOption::IpAddress() const noexcept
            {
                return mIpAddress;
            }

            Layer4ProtocolType Ipv4EndpointOption::L4Proto() const noexcept
            {
                return mL4Proto;
            }

            uint16_t Ipv4EndpointOption::Port() const noexcept
            {
                return mPort;
            }

            std::vector<uint8_t> Ipv4EndpointOption::Payload() const
            {
                auto _result = Option::BasePayload();

                helper::Ipv4Address::Inject(_result, mIpAddress);

                const uint8_t cReservedByte = 0x00;
                _result.push_back(cReservedByte);

                uint8_t _protocolByte = static_cast<uint8_t>(mL4Proto);
                _result.push_back(_protocolByte);

                helper::Inject(_result, mPort);

                return _result;
            }

            std::unique_ptr<Ipv4EndpointOption> Ipv4EndpointOption::CreateUnitcastEndpoint(
                bool discardable,
                helper::Ipv4Address ipAddress,
                Layer4ProtocolType protocol,
                uint16_t port) noexcept
            {
                std::unique_ptr<Ipv4EndpointOption> _result(
                    new Ipv4EndpointOption(
                        OptionType::IPv4Endpoint,
                        discardable,
                        ipAddress,
                        protocol,
                        port));

                return _result;
            }

            std::unique_ptr<Ipv4EndpointOption> Ipv4EndpointOption::CreateMulticastEndpoint(
                bool discardable,
                helper::Ipv4Address ipAddress,
                uint16_t port)
            {
                const uint8_t cMulticastOctetMin = 224;
                const uint8_t cMulticastOctetMax = 239;

                uint8_t _firstOctet = ipAddress.Octets[0];
                if ((_firstOctet < cMulticastOctetMin) ||
                    (_firstOctet > cMulticastOctetMax))
                {
                    throw std::invalid_argument("IP address is out of range.");
                }

                std::unique_ptr<Ipv4EndpointOption> _result(
                    new Ipv4EndpointOption(
                        OptionType::IPv4Multicast,
                        discardable,
                        ipAddress,
                        Layer4ProtocolType::Udp,
                        port));

                return _result;
            }

            std::unique_ptr<Ipv4EndpointOption> Ipv4EndpointOption::CreateSdEndpoint(
                bool discardable,
                helper::Ipv4Address ipAddress,
                Layer4ProtocolType protocol,
                uint16_t port) noexcept
            {
                std::unique_ptr<Ipv4EndpointOption> _result(
                    new Ipv4EndpointOption(OptionType::IPv4SdEndpoint,
                                           discardable,
                                           ipAddress,
                                           protocol,
                                           port));

                return _result;
            }

            std::unique_ptr<Ipv4EndpointOption> Ipv4EndpointOption::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset,
                OptionType type,
                bool discardable)
            {
                helper::Ipv4Address _ipAddress =
                    helper::Ipv4Address::Extract(payload, offset);

                // Apply the reserved byte length field offset
                offset++;

                auto _protocol =
                    static_cast<Layer4ProtocolType>(payload.at(offset++));

                uint16_t _port = helper::ExtractShort(payload, offset);

                switch (type)
                {
                case OptionType::IPv4Endpoint:
                    return CreateUnitcastEndpoint(
                        discardable, _ipAddress, _protocol, _port);

                case OptionType::IPv4Multicast:
                    return CreateMulticastEndpoint(
                        discardable, _ipAddress, _port);

                case OptionType::IPv4SdEndpoint:
                    return CreateSdEndpoint(
                        discardable, _ipAddress, _protocol, _port);

                default:
                    throw std::out_of_range(
                        "The option type does not belong to IPv4 endpoint option series.");
                }
            }
        }
    }
}