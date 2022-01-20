#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include "../../../../src/ara/com/option/option_deserializer.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            TEST(Ipv4EndpointOptionTest, UnitcastFactory)
            {
                const bool cDiscardable = true;
                const helper::Ipv4Address cIpAddress(127, 0, 0, 1);
                const Layer4ProtocolType cProtocol = Layer4ProtocolType::Tcp;
                const uint16_t cPort = 8080;
                const OptionType cType = OptionType::IPv4Endpoint;

                auto _option =
                    Ipv4EndpointOption::CreateUnitcastEndpoint(
                        cDiscardable, cIpAddress, cProtocol, cPort);

                EXPECT_EQ(_option->Discardable(), cDiscardable);
                EXPECT_EQ(_option->IpAddress(), cIpAddress);
                EXPECT_EQ(_option->L4Proto(), cProtocol);
                EXPECT_EQ(_option->Port(), cPort);
                EXPECT_EQ(_option->Type(), cType);
            }

            TEST(Ipv4EndpointOptionTest, MulticastFactory)
            {
                const bool cDiscardable = false;
                const helper::Ipv4Address cIpAddress(192, 168, 0, 1);
                const uint16_t cPort = 8090;

                EXPECT_THROW(
                    Ipv4EndpointOption::CreateMulticastEndpoint(
                        cDiscardable, cIpAddress, cPort),
                    std::invalid_argument);
            }

            TEST(Ipv4EndpointOptionTest, PayloadMethod)
            {
                const bool cDiscardable = true;
                const helper::Ipv4Address cIpAddress(192, 168, 1, 254);

                auto _option =
                    Ipv4EndpointOption::CreateSdEndpoint(cDiscardable, cIpAddress);

                const size_t cPayloadSize = 12;
                const std::array<uint8_t, cPayloadSize> cExpectedPayload =
                    {0x00, 0x09, 0x24, 0x01,
                     0xc0, 0xa8, 0x01, 0xfe,
                     0x00, 0x11, 0x77, 0x1a};

                auto _actualPayload = _option->Payload();

                bool _areEqual =
                    std::equal(
                        _actualPayload.begin(),
                        _actualPayload.end(),
                        cExpectedPayload.begin());

                EXPECT_TRUE(_areEqual);
            }

            TEST(Ipv4EndpointOptionTest, Deserializing)
            {
                const bool cDiscardable = true;
                const helper::Ipv4Address cIpAddress(127, 0, 0, 1);
                const Layer4ProtocolType cProtocol = Layer4ProtocolType::Tcp;
                const uint16_t cPort = 8080;
                const OptionType cType = OptionType::IPv4Endpoint;

                auto _originalOption =
                    Ipv4EndpointOption::CreateUnitcastEndpoint(
                        cDiscardable, cIpAddress, cProtocol, cPort);

                auto _payload = _originalOption->Payload();
                std::size_t _offset = 0;
                auto _deserializedOptionBase =
                    OptionDeserializer::Deserialize(_payload, _offset);

                auto _deserializedOption =
                    dynamic_cast<Ipv4EndpointOption *>(
                        _deserializedOptionBase.get());

                EXPECT_EQ(
                    _originalOption->Type(),
                    _deserializedOption->Type());

                EXPECT_EQ(
                    _originalOption->Discardable(),
                    _deserializedOption->Discardable());

                EXPECT_EQ(
                    _originalOption->IpAddress(),
                    _deserializedOption->IpAddress());

                EXPECT_EQ(
                    _originalOption->L4Proto(),
                    _deserializedOption->L4Proto());

                EXPECT_EQ(
                    _originalOption->Port(),
                    _deserializedOption->Port());
            }
        }
    }
}