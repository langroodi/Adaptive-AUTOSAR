#ifndef IPV4_ENDPOINT_OPTION_H
#define IPV4_ENDPOINT_OPTION_H

#include <stdexcept>
#include <memory>
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

                constexpr Ipv4EndpointOption(
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

            public:
                Ipv4EndpointOption() = delete;
                virtual uint16_t Length() const noexcept override;

                /// @brief Get IP address
                /// @returns IPv4 address
                helper::Ipv4Address IpAddress() const noexcept;

                /// @brief Get protocol
                /// @returns OSI layer-4 protocol
                Layer4ProtocolType L4Proto() const noexcept;

                /// @brief Get port
                /// @returns Network port number
                uint16_t Port() const noexcept;

                virtual std::vector<uint8_t> Payload() const override;

                /// @brief Unitcast endpoint factory
                /// @param discardable Indicates whether the option can be discarded or not
                /// @param ipAddress IP address
                /// @param protocol Layer-4 protocol
                /// @param port Port number
                /// @returns Unicast IPv4 endpoint
                static std::unique_ptr<Ipv4EndpointOption> CreateUnitcastEndpoint(
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    Layer4ProtocolType protocol,
                    uint16_t port) noexcept;

                /// @brief Multicast endpoint factory
                /// @param discardable Indicates whether the option can be discarded or not
                /// @param ipAddress IP address
                /// @param port Port number
                /// @returns Multicast IPv4 endpoint
                static std::unique_ptr<Ipv4EndpointOption> CreateMulticastEndpoint(
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    uint16_t port);

                /// @brief Service discovery factroy
                /// @param discardable Indicates whether the option can be discarded or not
                /// @param ipAddress IP address
                /// @param protocol Layer-4 protocol
                /// @param port Port number
                /// @returns Service discovery IPv4 endpoint
                static std::unique_ptr<Ipv4EndpointOption> CreateSdEndpoint(
                    bool discardable,
                    helper::Ipv4Address ipAddress,
                    Layer4ProtocolType protocol = cDefaultSdProtocol,
                    uint16_t port = cDefaultSdPort) noexcept;

                /// @brief Deserialize an option payload
                /// @param payload Serialized option payload byte array
                /// @param offset Deserializing offset in the payload
                /// @param type IPv4 endpoint option type
                /// @param discardable Indicates whether the option can be discarded or not
                /// @returns Deserialized option
                /// @throws std::out_of_range Throws when the option type is not an IPv4 endpoint
                static std::unique_ptr<Ipv4EndpointOption> Deserialize(
                    const std::vector<uint8_t> &payload,
                    std::size_t &offset,
                    OptionType type,
                    bool discardable);
            };
        }
    }
}

#endif