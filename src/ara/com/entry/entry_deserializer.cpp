#include "./entry_deserializer.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            std::unique_ptr<Entry> EntryDeserializer::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset,
                uint8_t &numberOfFirstOptions,
                uint8_t &numberOfSecondOptions)
            {
                auto _type = static_cast<EntryType>(payload.at(offset++));

                // Apply the first and the second options' indices fields
                offset += 2;

                uint8_t optionsNumbers = payload.at(offset++);
                numberOfFirstOptions = optionsNumbers >> Entry::cOptionSizeBitLength;
                const uint8_t cSecondOptionsNumberMask = 0x0f;
                numberOfSecondOptions = optionsNumbers & cSecondOptionsNumberMask;

                uint16_t _serviceId = helper::ExtractShort(payload, offset);
                uint16_t _instanceId = helper::ExtractShort(payload, offset);

                const uint8_t cTTLSizeBitLength = 24;
                const uint32_t cTTLMask = 0x00ffffff;
                uint32_t _combinedMajorVersionTtl =
                    helper::ExtractInteger(payload, offset);

                uint8_t _majorVersion =
                    static_cast<uint8_t>(_combinedMajorVersionTtl >> cTTLSizeBitLength);

                uint32_t _ttl = _combinedMajorVersionTtl & cTTLMask;

                switch (_type)
                {
                case EntryType::Finding:
                case EntryType::Offering:
                    return ServiceEntry::Deserialize(
                        payload,
                        offset,
                        _type,
                        _serviceId,
                        _instanceId,
                        _ttl,
                        _majorVersion);

                case EntryType::Subscribing:
                case EntryType::Acknowledging:
                    return EventgroupEntry::Deserialize(
                        payload,
                        offset,
                        _type,
                        _serviceId,
                        _instanceId,
                        _ttl,
                        _majorVersion);

                default:
                    throw std::out_of_range(
                        "Entry type is not supported for deserializing.");
                }
            }
        }
    }
}