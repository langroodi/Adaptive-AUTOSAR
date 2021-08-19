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
                    const std::uint32_t cMessageId = 0xffff8100;
                    const std::uint16_t cClientId = 0x0000;
                    const std::uint8_t cProtocolVersion = 0x01;
                    const std::uint8_t cInterfaceVersion = 0x01;
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Notification;

                    bool mRebooted;
                    std::vector<entry::Entry> mEntries;

                    std::uint32_t getEntriesLength() const noexcept;
                    std::uint32_t getOptionsLength() const noexcept;

                public:
                    SomeIpSdMessage();

                    /// @brief Get entries
                    /// @returns Exisiting message entries
                    const std::vector<entry::Entry> &Entries() const noexcept;

                    /// @brief Add an entry
                    /// @param entry Entry to be added
                    void AddEntry(entry::Entry &&entry);

                    virtual std::uint32_t Length() const noexcept override;

                    virtual void SetSessionId(std::uint16_t sessionId) override;

                    virtual bool IncrementSessionId() noexcept override;

                    virtual const std::vector<std::uint8_t> &Payload() override;
                };
            }
        }
    }
}
#endif