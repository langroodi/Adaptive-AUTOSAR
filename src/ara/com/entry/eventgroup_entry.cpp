#include "./eventgroup_entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            EventgroupEntry::EventgroupEntry(EntryType type,
                                             std::uint16_t serviceId,
                                             std::uint16_t instanceId,
                                             std::uint32_t ttl,
                                             sts::uint8_t majorVersion,
                                             std::uint16_t eventgroupId) noexcept : Entry(type, serviceId, instanceId, ttl, majorVersion),
                                                                                    mEventgroupId{eventgroupId}
            {
            }

            std::uint16_t EventgroupEntry::EvengroupId() const noexcept
            {
                return mEventgroupId;
            }

            EventgroupEntry EventgroupEntry::CreateSubscribeEventEntry(
                std::uint16_t serviceId,
                std::uint16_t instanceId,
                sts::uint8_t majorVersion,
                std::uint32_t eventgroupId) noexcept
            {
                const EntryType cSubscribeEventEntry = EntryType::Subscribing;
                const std::uint32_t cSubscribeEventTTL = 0xffffff;

                EventgroupEntry _result(
                    cSubscribeEventEntry,
                    serviceId,
                    instanceId,
                    cSubscribeEventTTL,
                    majorVersion,
                    eventgroupId);

                return _result;
            }

            EventgroupEntry EventgroupEntry::CreateUnsubscribeEventEntry(
                std::uint16_t serviceId,
                std::uint16_t instanceId,
                sts::uint8_t majorVersion,
                std::uint32_t eventgroupId) noexcept
            {
                const EntryType cSubscribeEventEntry = EntryType::Subscribing;
                const std::uint32_t cUnsubscribeEventTTL = 0x000000;

                EventgroupEntry _result(
                    cSubscribeEventEntry,
                    serviceId,
                    instanceId,
                    cSubscribeEventTTL,
                    majorVersion,
                    eventgroupId);

                return _result;
            }

            EventgroupEntry EventgroupEntry::CreateAcknowledgeEntry(
                const EventgroupEntry &eventgroupEntry) noexcept
            {
                const EntryType cAcknowledgetEntry = EntryType::Acknowledging;

                EventgroupEntry _result(
                    cAcknowledgetEntry,
                    eventgroupEntry.SeriveId(),
                    eventgroupEntry.InstanceId(),
                    eventgroupEntry.TTL(),
                    eventgroupEntry.MajorVersion(),
                    eventgroupEntry.EventgroupId());

                return _result;
            }

            EventgroupEntry EventgroupEntry::CreateNegativeAcknowledgeEntry(
                const EventgroupEntry &eventgroupEntry) noexcept
            {
                const EntryType cAcknowledgetEntry = EntryType::Acknowledging;
                const std::uint32_t cNackTTL = 0x000000;

                EventgroupEntry _result(
                    cAcknowledgetEntry,
                    eventgroupEntry.SeriveId(),
                    eventgroupEntry.InstanceId(),
                    cNackTTL,
                    eventgroupEntry.MajorVersion(),
                    eventgroupEntry.EventgroupId());

                return _result;
            }
        }
    }
}