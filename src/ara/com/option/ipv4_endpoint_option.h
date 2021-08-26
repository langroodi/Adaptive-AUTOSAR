#ifndef IPV4_ENDPOINT_OPTION_H
#define IPv4_ENDPOINT_OPTION_H

#include <stdexcept>
#include "./option.h"
#include "../helper/ipv4_address.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            /// @brief IPv4 endpoint option for both generic and service discovery purposes
            class Ipv4EndpointOption : public Option
            {
            private:
                static const Layer4ProtocolType cDefaultSdProtocol = Layer4ProtocolType::Udp;
                static const uint16_t cDefaultSdPort = 30490;

                helper::Ipv4Address mIpAddress;
                Layer4ProtocolType mL4Proto;
                uint16_t mPort;

                Ipv4EndpointOption(
                    OptionType type,
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    Layer4ProtocolType protocol,
                    uint16_t port) noexcept;

            public:
                Ipv4EndpointOption() = delete;
                virtual uint16_t Length() override;

                /// @brief Get IP address
                /// @returns IPv4 address
                helper::Ipv4Address IpAddress();

                /// @brief Get protocol
                /// @returns OSI layer-4 protocol
                Layer4ProtocolType L4Proto();

                /// @brief Get port
                /// @returns Network port number
                uint16_t Port();

                virtual std::vector<uint8_t> Payload() override;

                /// @brief Unitcast endpoint factory
                /// @param discardable Indicates whether the option can be discarded or not
                /// @param ipAddress IP address
                /// @param protocol Layer-4 protocol
                /// @param port Port number
                /// @returns Unicast IPv4 endpoint
                static Ipv4EndpointOption CreateUnitcastEndpoint(
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    Layer4ProtocolType protocol,
                    uint16_t port) noexcept;

                /// @brief Multicast endpoint factory
                /// @param discardable Indicates whether the option can be discarded or not
                /// @param ipAddress IP address
                /// @param port Port number
                /// @returns Multicast IPv4 endpoint
                static Ipv4EndpointOption CreateMulticastEndpoint(
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    uint16_t port);

                /// @brief Service discovery factroy
                /// @param discardable Indicates whether the option can be discarded or not
                /// @param ipAddress IP address
                /// @param protocol Layer-4 protocol
                /// @param port Port number
                /// @returns Service discovery IPv4 endpoint
                static Ipv4EndpointOption CreateSdEndpoint(
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    Layer4ProtocolType protocol = cDefaultSdProtocol,
                    uint16_t port = cDefaultSdPort) noexcept;
            };
        }
    }
}

#endif