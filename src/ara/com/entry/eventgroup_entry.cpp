#include "./eventgroup_entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            EventgroupEntry::EventgroupEntry(EntryType type,
                                             uint16_t serviceId,
                                             uint16_t instanceId,
                                             uint32_t ttl,
                                             uint8_t majorVersion,
                                             uint16_t eventgroupId) noexcept : Entry(type, serviceId, instanceId, ttl, majorVersion),
                                                                               mEventgroupId{eventgroupId}
            {
            }

            bool EventgroupEntry::isAcknowledge() const noexcept
            {
                bool _result =
                    (Type() == EntryType::Acknowledging) && (TTL() > cNackTTL);

                return _result;
            }

            bool EventgroupEntry::ValidateOption(
                std::shared_ptr<const option::Option> option) const noexcept
            {
                bool _result = Entry::ValidateOption(option);

                if (_result)
                {
                    switch (option->Type())
                    {
                    case option::OptionType::IPv4Endpoint:
                    case option::OptionType::IPv6Endpoint:
                    {
                        // Endpoint option is allowed only eventgroup subscription entries.
                        _result = (Type() == EntryType::Subscribing);
                        break;
                    }
                    case option::OptionType::IPv4Multicast:
                    case option::OptionType::IPv6Multicast:
                    {
                        // Multicast option is not allowed in eventgroup entries expect acknowledgement.
                        if (isAcknowledge())
                        {
                            _result = !ContainsOption(option->Type());
                        }
                        else
                        {
                            _result = false;
                        }

                        break;
                    }
                    default:
                    {
                        _result = false;
                        break;
                    }
                    }
                }

                return _result;
            }

            uint16_t EventgroupEntry::EventgroupId() const noexcept
            {
                return mEventgroupId;
            }

            std::vector<uint8_t> EventgroupEntry::Payload(uint8_t &optionIndex) const
            {
                std::vector<uint8_t> _result = Entry::BasePayload(optionIndex);

                // Enabled Inistal Data Request Flag without any counter
                const uint16_t cEventgroupFlag = 0x0080;
                helper::Inject(_result, cEventgroupFlag);
                helper::Inject(_result, mEventgroupId);

                return _result;
            }

            EventgroupEntry EventgroupEntry::CreateSubscribeEventEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint16_t eventgroupId) noexcept
            {
                const EntryType cSubscribeEventEntry = EntryType::Subscribing;
                const uint32_t cSubscribeEventTTL = 0xffffff;

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
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint16_t eventgroupId) noexcept
            {
                const EntryType cSubscribeEventEntry = EntryType::Subscribing;
                const uint32_t cUnsubscribeEventTTL = 0x000000;

                EventgroupEntry _result(
                    cSubscribeEventEntry,
                    serviceId,
                    instanceId,
                    cUnsubscribeEventTTL,
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
                    eventgroupEntry.ServiceId(),
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

                EventgroupEntry _result(
                    cAcknowledgetEntry,
                    eventgroupEntry.ServiceId(),
                    eventgroupEntry.InstanceId(),
                    cNackTTL,
                    eventgroupEntry.MajorVersion(),
                    eventgroupEntry.EventgroupId());

                return _result;
            }
        }
    }
}