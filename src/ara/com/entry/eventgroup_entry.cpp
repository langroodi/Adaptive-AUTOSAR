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
                                             uint8_t counter,
                                             uint16_t eventgroupId) : Entry(type, serviceId, instanceId, ttl, majorVersion),
                                                                      mCounter{counter},
                                                                      mEventgroupId{eventgroupId}
            {
                const uint8_t cCounterMax = 0x0f;
                if (counter > cCounterMax)
                {
                    throw std::out_of_range("The counter is out of range.");
                }
            }

            EventgroupEntry::EventgroupEntry(EventgroupEntry &&entry) : Entry{std::move(entry)},
                                                                        mCounter{entry.mCounter},
                                                                        mEventgroupId{entry.mEventgroupId}
            {
            }

            EventgroupEntry &EventgroupEntry::operator=(EventgroupEntry &&other)
            {
                Entry::operator=(std::move(other));
                mCounter = other.mCounter;
                mEventgroupId = other.mEventgroupId;

                return *this;
            }

            bool EventgroupEntry::isAcknowledge() const noexcept
            {
                bool _result =
                    (Type() == EntryType::Acknowledging) && (TTL() > cNackTTL);

                return _result;
            }

            bool EventgroupEntry::ValidateOption(
                const option::Option *option) const noexcept
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

            uint8_t EventgroupEntry::Counter() const noexcept
            {
                return mCounter;
            }

            uint16_t EventgroupEntry::EventgroupId() const noexcept
            {
                return mEventgroupId;
            }

            std::vector<uint8_t> EventgroupEntry::Payload(uint8_t &optionIndex) const
            {
                std::vector<uint8_t> _result = Entry::BasePayload(optionIndex);

                uint16_t _eventgroupFlag = static_cast<uint16_t>(mCounter);
                helper::Inject(_result, _eventgroupFlag);
                helper::Inject(_result, mEventgroupId);

                return _result;
            }

            std::unique_ptr<EventgroupEntry> EventgroupEntry::CreateSubscribeEventEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint8_t counter,
                uint16_t eventgroupId)
            {
                const EntryType cSubscribeEventEntry = EntryType::Subscribing;
                const uint32_t cSubscribeEventTTL = 0xffffff;

                std::unique_ptr<EventgroupEntry> _result(
                    new EventgroupEntry(
                        cSubscribeEventEntry,
                        serviceId,
                        instanceId,
                        cSubscribeEventTTL,
                        majorVersion,
                        counter,
                        eventgroupId));

                return _result;
            }

            std::unique_ptr<EventgroupEntry> EventgroupEntry::CreateUnsubscribeEventEntry(
                uint16_t serviceId,
                uint16_t instanceId,
                uint8_t majorVersion,
                uint8_t counter,
                uint16_t eventgroupId)
            {
                const EntryType cSubscribeEventEntry = EntryType::Subscribing;

                std::unique_ptr<EventgroupEntry> _result(
                    new EventgroupEntry(
                        cSubscribeEventEntry,
                        serviceId,
                        instanceId,
                        cUnsubscribeEventTTL,
                        majorVersion,
                        counter,
                        eventgroupId));

                return _result;
            }

            std::unique_ptr<EventgroupEntry> EventgroupEntry::CreateAcknowledgeEntry(
                const EventgroupEntry *eventgroupEntry)
            {
                const EntryType cAcknowledgetEntry = EntryType::Acknowledging;

                std::unique_ptr<EventgroupEntry> _result(
                    new EventgroupEntry(
                        cAcknowledgetEntry,
                        eventgroupEntry->ServiceId(),
                        eventgroupEntry->InstanceId(),
                        eventgroupEntry->TTL(),
                        eventgroupEntry->MajorVersion(),
                        eventgroupEntry->Counter(),
                        eventgroupEntry->EventgroupId()));

                return _result;
            }

            std::unique_ptr<EventgroupEntry> EventgroupEntry::CreateNegativeAcknowledgeEntry(
                const EventgroupEntry *eventgroupEntry)
            {
                const EntryType cAcknowledgetEntry = EntryType::Acknowledging;

                std::unique_ptr<EventgroupEntry> _result(
                    new EventgroupEntry(
                        cAcknowledgetEntry,
                        eventgroupEntry->ServiceId(),
                        eventgroupEntry->InstanceId(),
                        cNackTTL,
                        eventgroupEntry->MajorVersion(),
                        eventgroupEntry->Counter(),
                        eventgroupEntry->EventgroupId()));

                return _result;
            }

            std::unique_ptr<EventgroupEntry> EventgroupEntry::Deserialize(
                const std::vector<uint8_t> &payload,
                std::size_t &offset,
                EntryType type,
                uint16_t serviceId,
                uint16_t instanceId,
                uint32_t ttl,
                uint8_t majorVersion)
            {
                // Apply the reserved byte offset
                offset++;

                uint8_t _counter = payload.at(offset++);
                uint16_t _eventgroupId = helper::ExtractShort(payload, offset);

                switch (type)
                {
                case EntryType::Subscribing:
                {
                    if (ttl > cUnsubscribeEventTTL)
                    {
                        return CreateSubscribeEventEntry(
                            serviceId, instanceId, majorVersion, _counter, _eventgroupId);
                    }
                    else
                    {
                        return CreateUnsubscribeEventEntry(
                            serviceId, instanceId, majorVersion, _counter, _eventgroupId);
                    }
                }

                case EntryType::Acknowledging:
                {
                    std::unique_ptr<EventgroupEntry> _result(
                        new EventgroupEntry(
                            type,
                            serviceId,
                            instanceId,
                            ttl,
                            majorVersion,
                            _counter,
                            _eventgroupId));

                    return _result;
                }

                default:
                    throw std::out_of_range(
                        "The entry type does not belong to service entry series.");
                }
            }
        }
    }
}