#ifndef SOMEIP_SD_MESSAGE_H
#define SOMEIP_SD_MESSAGE_H

#include <utility>
#include "../someip_message.h"
#include "../../entry/entry.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery message
                class SomeIpSdMessage : public SomeIpMessage
                {
                private:
                    static const uint32_t cMessageId = 0xffff8100;
                    static const uint16_t cClientId = 0x0000;
                    static const uint8_t cProtocolVersion = 0x01;
                    static const uint8_t cInterfaceVersion = 0x01;
                    static const SomeIpMessageType cMessageType = SomeIpMessageType::Notification;
                    static const uint32_t cRebootedFlag = 0xc0000000;
                    static const uint32_t cNotRebootedFlag = 0x40000000;

                    bool mRebooted;
                    std::vector<std::unique_ptr<entry::Entry>> mEntries;

                    uint32_t getEntriesLength() const noexcept;
                    uint32_t getOptionsLength() const noexcept;

                public:
                    SomeIpSdMessage();
                    SomeIpSdMessage(SomeIpSdMessage&& other);

                    SomeIpSdMessage& operator=(SomeIpSdMessage&& other);

                    /// @brief Get entries
                    /// @returns Exisiting message entries
                    const std::vector<std::unique_ptr<entry::Entry>> &Entries() const noexcept;

                    /// @brief Add an entry
                    /// @param entry Entry to be added
                    void AddEntry(std::unique_ptr<entry::Entry> entry);

                    virtual uint32_t Length() const noexcept override;

                    virtual void SetSessionId(uint16_t sessionId) override;

                    virtual bool IncrementSessionId() noexcept override;

                    virtual std::vector<uint8_t> Payload() const override;

                    /// @brief Deserialize a SOME/IP SD message payload
                    /// @param payload Serialized SOME/IP message payload byte array
                    /// @returns SOME/IP SD message filled by deserializing the payload
                    /// @throws std::out_of_range Throws when the payload is corrupted
                    static SomeIpSdMessage Deserialize(const std::vector<uint8_t> &payload);
                };
            }
        }
    }
}
#endif