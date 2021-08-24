#include "./service_entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            ServiceEntry::ServiceEntry(EntryType type,
                                       uint16_t serviceId,
                                       uint16_t instanceId,
                                       uint32_t ttl,
                                       uint8_t majorVersion,
                                       uint32_t minorVersion) noexcept : Entry(type, serviceId, instanceId, ttl, majorVersion),
                                                                         mMinorVersion{minorVersion}
            {
            }

            bool ServiceEntry::ValidateOption(
                const option::Option *option) const noexcept
            {
                bool _result = Entry::ValidateOption(option);

                // Multicast option is not allowed in service entries.
                _result &=
                    (option->Type() == option::OptionType::IPv4Multicast) ||
                    (option->Type() == option::OptionType::IPv6Multicast);

                return _result;
            }

            uint32_t ServiceEntry::MinorVersion() const noexcept
            {
                return mMinorVersion;
            }

            std::vector<uint8_t> ServiceEntry::Payload(uint8_t &optionIndex)
            {
                std::vector<uint8_t> _result = Entry::Payload(optionIndex);
                helper::Inject(_result, mMinorVersion);

                return _result;
            }

            ServiceEntry ServiceEntry::CreateFindServiceEntry(
                uint16_t serviceId,
                uint32_t ttl,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint32_t minorVersion)
            {
                const EntryType cFindServiceEntry = EntryType::Finding;

                if (ttl == 0)
                {
                    throw std::invalid_argument("TTL cannot be zero.");
                }

                ServiceEntry _result(
                    cFindServiceEntry,
                    serviceId,
                    instanceId,
                    ttl,
                    majorVersion,
                    minorVersion);

                return _result;
            }

            ServiceEntry ServiceEntry::CreateOfferServiceEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint32_t minorVersion) noexcept
            {
                const EntryType cOfferServiceEntry = EntryType::Offering;
                const uint32_t cOfferServiceTTL = 0xffffff;

                ServiceEntry _result(
                    cOfferServiceEntry,
                    serviceId,
                    instanceId,
                    cOfferServiceTTL,
                    majorVersion,
                    minorVersion);

                return _result;
            }

            ServiceEntry ServiceEntry::CreateStopOfferEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint32_t minorVersion) noexcept
            {
                const EntryType cOfferServiceEntry = EntryType::Offering;
                const uint32_t cStopOfferTTL = 0x000000;

                ServiceEntry _result(
                    cOfferServiceEntry,
                    serviceId,
                    instanceId,
                    cStopOfferTTL,
                    majorVersion,
                    minorVersion);

                return _result;
            }
        }
    }
}