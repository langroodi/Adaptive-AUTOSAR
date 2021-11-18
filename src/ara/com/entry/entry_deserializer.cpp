#include "./entry_deserializer.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            std::shared_ptr<Entry> EntryDeserializer::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset)
            {
                auto _type = static_cast<EntryType>(payload.at(offset++));

                // Apply the first and the second options' indices and numbers fields
                offset += 3;

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