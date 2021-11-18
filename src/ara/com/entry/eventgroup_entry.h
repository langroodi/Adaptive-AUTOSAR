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

                uint16_t mEventgroupId;

                EventgroupEntry(EntryType type,
                                uint16_t serviceId,
                                uint16_t instanceId,
                                uint32_t ttl,
                                uint8_t majorVersion,
                                uint16_t eventgroupId) noexcept;

                bool isAcknowledge() const noexcept;

            protected:
                virtual bool ValidateOption(
                    std::shared_ptr<const option::Option> option) const noexcept override;

            public:
                EventgroupEntry() = delete;

                /// @brief Get event-group ID
                /// @returns Event-group ID for subscription/unsubscription
                uint16_t EventgroupId() const noexcept;

                virtual std::vector<uint8_t> Payload(uint8_t &optionIndex) const override;

                /// @brief Subscribe to an event-group entry factory
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param eventgroupId Event-group in interest ID
                /// @returns Subscribe event-group entry
                static std::shared_ptr<EventgroupEntry> CreateSubscribeEventEntry(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint16_t eventgroupId) noexcept;

                /// @brief Unsubscribe from an event-group entry factory
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param eventgroupId Event-group in interest ID
                /// @returns Unsubscribe event-group entry
                static std::shared_ptr<EventgroupEntry> CreateUnsubscribeEventEntry(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint16_t eventgroupId) noexcept;

                /// @brief Positive acknowledge of an event-group entry factory
                /// @param eventgroupEntry Received subscribe event-group entry
                /// @returns Acknowledge event-group subscription entry
                static std::shared_ptr<EventgroupEntry> CreateAcknowledgeEntry(
                    std::shared_ptr<EventgroupEntry> eventgroupEntry) noexcept;

                /// @brief Negative acknowledge of an event-group entry factory
                /// @param eventgroupEntry Received subscribe event-group entry
                /// @returns Negative acknowledge event-group subscription entry
                static std::shared_ptr<EventgroupEntry> CreateNegativeAcknowledgeEntry(
                    std::shared_ptr<EventgroupEntry> eventgroupEntry) noexcept;

                /// @brief Deserialize a entry payload
                /// @param payload Serialized entry payload byte array
                /// @param offset Deserializing offset in the payload
                /// @param type Entry type
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param ttl Entry time to live
                /// @param majorVersion Service in interest major version
                /// @returns Shared pointer to the entry which is created while deserializing
                /// @throws std::out_of_range Throws when the entry type is not an event-group entry
                static std::shared_ptr<EventgroupEntry> Deserialize(
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