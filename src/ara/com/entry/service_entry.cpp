#include "./service_entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            ServiceEntry::ServiceEntry(EntryType type,
                                       std::uint16_t serviceId,
                                       std::uint16_t instanceId,
                                       std::uint32_t ttl,
                                       sts::uint8_t majorVersion,
                                       std::uint32_t minorVersion) noexcept : Entry(type, serviceId, instanceId, ttl, majorVersion),
                                                                              mMinorVersion{minorVersion}
            {
            }

            std::uint32_t ServiceEntry::MinorVersion() const noexcept
            {
                return mMinorVersion;
            }

            const std::vector<std::uint8_t> &ServiceEntry::Payload(std::uint8_t &optionIndex)
            {
                std::vector<std::uint8_t> _result = Entry::Payload(optionIndex);
                someip::SomeIpMessage::Inject(_result, mMinorVersion);

                return _result;
            }

            ServiceEntry ServiceEntry::CreateFindServiceEntry(
                std::uint16_t serviceId,
                std::uint32_t ttl,
                std::uint16_t instanceId,
                sts::uint8_t majorVersion,
                std::uint32_t minorVersion) noexcept
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
                std::uint16_t serviceId,
                std::uint16_t instanceId,
                sts::uint8_t majorVersion,
                std::uint32_t minorVersion) noexcept
            {
                const EntryType cOfferServiceEntry = EntryType::Offering;
                const std::uint32_t cOfferServiceTTL = 0xffffff;

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
                std::uint16_t serviceId,
                std::uint16_t instanceId,
                sts::uint8_t majorVersion,
                std::uint32_t minorVersion) noexcept
            {
                const EntryType cOfferServiceEntry = EntryType::Offering;
                const std::uint32_t cStopOfferTTL = 0x000000;

                ServiceEntry _result(
                    cOfferServiceEntry,
                    serviceId,
                    instanceId,
                    cOfferServiceTTL,
                    majorVersion,
                    minorVersion);

                return _result;
            }
        }
    }
}