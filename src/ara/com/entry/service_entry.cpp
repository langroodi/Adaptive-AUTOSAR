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

            ServiceEntry::ServiceEntry(ServiceEntry &&other) : Entry{std::move(other)},
                                                               mMinorVersion{other.mMinorVersion}
            {
            }

            ServiceEntry &ServiceEntry::operator=(ServiceEntry &&other)
            {
                Entry::operator=(std::move(other));
                mMinorVersion = other.mMinorVersion;

                return *this;
            }

            bool ServiceEntry::ValidateOption(
                const option::Option *option) const noexcept
            {
                bool _result = Entry::ValidateOption(option);

                // Multicast option is not allowed in service entries.
                _result &=
                    (option->Type() != option::OptionType::IPv4Multicast) &&
                    (option->Type() != option::OptionType::IPv6Multicast);

                return _result;
            }

            uint32_t ServiceEntry::MinorVersion() const noexcept
            {
                return mMinorVersion;
            }

            std::vector<uint8_t> ServiceEntry::Payload(uint8_t &optionIndex) const
            {
                std::vector<uint8_t> _result = Entry::BasePayload(optionIndex);
                helper::Inject(_result, mMinorVersion);

                return _result;
            }

            std::unique_ptr<ServiceEntry> ServiceEntry::CreateFindServiceEntry(
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

                std::unique_ptr<ServiceEntry> _result(
                    new ServiceEntry(
                        cFindServiceEntry,
                        serviceId,
                        instanceId,
                        ttl,
                        majorVersion,
                        minorVersion));

                return _result;
            }

            std::unique_ptr<ServiceEntry> ServiceEntry::CreateOfferServiceEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint32_t minorVersion,
                uint32_t ttl)
            {
                const EntryType cOfferServiceEntry = EntryType::Offering;

                if (ttl == 0)
                {
                    throw std::invalid_argument("TTL cannot be zero.");
                }

                std::unique_ptr<ServiceEntry> _result(
                    new ServiceEntry(
                        cOfferServiceEntry,
                        serviceId,
                        instanceId,
                        ttl,
                        majorVersion,
                        minorVersion));

                return _result;
            }

            std::unique_ptr<ServiceEntry> ServiceEntry::CreateStopOfferEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint32_t minorVersion) noexcept
            {
                const EntryType cOfferServiceEntry = EntryType::Offering;
                const uint32_t cStopOfferTTL = 0x000000;

                std::unique_ptr<ServiceEntry> _result(
                    new ServiceEntry(
                        cOfferServiceEntry,
                        serviceId,
                        instanceId,
                        cStopOfferTTL,
                        majorVersion,
                        minorVersion));

                return _result;
            }

            std::unique_ptr<ServiceEntry> ServiceEntry::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset,
                EntryType type,
                uint16_t serviceId,
                uint16_t instanceId,
                uint32_t ttl,
                uint8_t majorVersion)
            {
                uint32_t _minorVersion = helper::ExtractInteger(payload, offset);

                switch (type)
                {
                case EntryType::Finding:
                    return CreateFindServiceEntry(
                        serviceId, ttl, instanceId, majorVersion, _minorVersion);

                case EntryType::Offering:
                {
                    if (ttl > 0)
                    {
                        return CreateOfferServiceEntry(
                            serviceId, instanceId, majorVersion, _minorVersion, ttl);
                    }
                    else
                    {
                        return CreateStopOfferEntry(
                            serviceId, instanceId, majorVersion, _minorVersion);
                    }
                }

                default:
                    throw std::out_of_range(
                        "The entry type does not belong to service entry series.");
                }
            }
        }
    }
}