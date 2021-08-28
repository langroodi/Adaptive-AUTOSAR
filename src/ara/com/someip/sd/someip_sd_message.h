#ifndef SOMEIP_SD_MESSAGE_H
#define SOMEIP_SD_MESSAGE_H

#include <stdint.h>
#include <vector>
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

                    bool mRebooted;
                    std::vector<entry::Entry *> mEntries;

                    uint32_t getEntriesLength() const noexcept;
                    uint32_t getOptionsLength() const noexcept;

                public:
                    SomeIpSdMessage();

                    /// @brief Get entries
                    /// @returns Exisiting message entries
                    const std::vector<entry::Entry *> &Entries() const noexcept;

                    /// @brief Add an entry
                    /// @param entry Entry to be added
                    void AddEntry(entry::Entry *entry);

                    virtual uint32_t Length() const noexcept override;

                    virtual void SetSessionId(uint16_t sessionId) override;

                    virtual bool IncrementSessionId() noexcept override;

                    virtual std::vector<uint8_t> Payload() const override;
                };
            }
        }
    }
}
#endif