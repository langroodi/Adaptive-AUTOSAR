#include <gtest/gtest.h>
#include "../../../../src/ara/com/option/ipv4_endpoint_option.h"

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

                EXPECT_EQ(_option.Discardable(), cDiscardable);
                EXPECT_EQ(_option.IpAddress(), cIpAddress);
                EXPECT_EQ(_option.L4Proto(), cProtocol);
                EXPECT_EQ(_option.Port(), cPort);
                EXPECT_EQ(_option.Type(), cType);
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
        }
    }
}