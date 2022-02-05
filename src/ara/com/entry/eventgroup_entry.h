#ifndef EVENTGROUP_ENTRY_H
#define EVENTGROUP_ENTRY_H

#include "./entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            /// @brief Entry to subscribe/unsubscribe to/from an event-group
            class EventgroupEntry : public Entry
            {
            private:
                static const uint32_t cNackTTL = 0x000000;
                static const uint32_t cUnsubscribeEventTTL = 0x000000;

                uint8_t mCounter;
                uint16_t mEventgroupId;

                EventgroupEntry(EntryType type,
                                uint16_t serviceId,
                                uint16_t instanceId,
                                uint32_t ttl,
                                uint8_t majorVersion,
                                uint8_t counter,
                                uint16_t eventgroupId);

                bool isAcknowledge() const noexcept;

            protected:
                virtual bool ValidateOption(
                    const option::Option *option) const noexcept override;

            public:
                EventgroupEntry() = delete;
                EventgroupEntry(EventgroupEntry &&other);

                EventgroupEntry &operator=(EventgroupEntry &&other);

                /// @brief Get the subscriber counter
                /// @returns A 4-bit unsinged integer wrapped as a byte
                uint8_t Counter() const noexcept;

                /// @brief Get event-group ID
                /// @returns Event-group ID for subscription/unsubscription
                uint16_t EventgroupId() const noexcept;

                virtual std::vector<uint8_t> Payload(uint8_t &optionIndex) const override;

                /// @brief Subscribe to an event-group entry factory
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param counter Counter to distinguish between subscribers
                /// @param eventgroupId Event-group in interest ID
                /// @returns Subscribe event-group entry
                /// @throws std::out_of_range Throws if counter is greater than 4 bits
                static std::unique_ptr<EventgroupEntry> CreateSubscribeEventEntry(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint8_t counter,
                    uint16_t eventgroupId);

                /// @brief Unsubscribe from an event-group entry factory
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param counter Counter to distinguish between subscribers
                /// @param eventgroupId Event-group in interest ID
                /// @returns Unsubscribe event-group entry
                /// @throws std::out_of_range Throws if counter is greater than 4 bits
                static std::unique_ptr<EventgroupEntry> CreateUnsubscribeEventEntry(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint8_t counter,
                    uint16_t eventgroupId);

                /// @brief Positive acknowledge of an event-group entry factory
                /// @param eventgroupEntry Received subscribe event-group entry
                /// @returns Acknowledge event-group subscription entry
                static std::unique_ptr<EventgroupEntry> CreateAcknowledgeEntry(
                    const EventgroupEntry *eventgroupEntry);

                /// @brief Negative acknowledge of an event-group entry factory
                /// @param eventgroupEntry Received subscribe event-group entry
                /// @returns Negative acknowledge event-group subscription entry
                static std::unique_ptr<EventgroupEntry> CreateNegativeAcknowledgeEntry(
                    const EventgroupEntry *eventgroupEntry);

                /// @brief Deserialize a entry payload
                /// @param payload Serialized entry payload byte array
                /// @param offset Deserializing offset in the payload
                /// @param type Entry type
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param ttl Entry time to live
                /// @param majorVersion Service in interest major version
                /// @returns Deserialized entry
                /// @throws std::out_of_range Throws when the entry type is not an event-group entry
                static std::unique_ptr<EventgroupEntry> Deserialize(
                    const std::vector<uint8_t> &payload,
                    std::size_t &offset,
                    EntryType type,
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint32_t ttl,
                    uint8_t majorVersion);
            };
        }
    }
}

#endif