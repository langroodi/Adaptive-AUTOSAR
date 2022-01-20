#include "./option_deserializer.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            std::unique_ptr<Option> OptionDeserializer::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset)
            {
                // Apply the option length field offset
                offset += 2;

                auto _type = static_cast<OptionType>(payload.at(offset++));
                auto _discardable = static_cast<bool>(payload.at(offset++));

                switch (_type)
                {
                case OptionType::IPv4Endpoint:
                case OptionType::IPv4Multicast:
                case OptionType::IPv4SdEndpoint:
                    return Ipv4EndpointOption::Deserialize(
                        payload, offset, _type, _discardable);

                case OptionType::LoadBalancing:
                    return LoadBalancingOption::Deserialize(
                        payload, offset, _discardable);

                default:
                    throw std::out_of_range(
                        "Option type is not supported for deserializing.");
                }
            }
        }
    }
}