#include "./someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                SomeIpSdMessage::SomeIpSdMessage() : SomeIpMessage(
                                                         cMessageId,
                                                         cClientId,
                                                         cProtocolVersion,
                                                         cInterfaceVersion,
                                                         cMessageType),
                                                     mRebooted{true}
                {
                }

                std::uint32_t SomeIpSdMessage::getEntriesLength() const noexcept
                {
                    const std::uint32_t cEntrySize = 16;
                    std::uint32_t _numberOfEntries = mEntries.size();
                    std::uint32_t _result = _numberOfEntries * cEntrySize;

                    return _result;
                }

                std::uint32_t SomeIpSdMessage::getOptionsLength() const noexcept
                {
                    const std::uint32_t cOptionLengthFieldSize = 2;
                    std::uint32_t _result = 0;

                    for (auto entry : mEntries)
                    {
                        for (auto firstOption : entry.FirstOptions())
                        {
                            _result += +firstOption->Length();
                        }

                        for (auto secondOption : entry.SecondOptions())
                        {
                            _result += cOptionLengthFieldSize + secondOption->Length();
                        }
                    }

                    return _result;
                }

                const std::vector<entry::Entry> &SomeIpSdMessage::Entries() const noexcept
                {
                    return mEntries;
                }

                void SomeIpSdMessage::AddEntry(entry::Entry &&entry)
                {
                    mEntries.push_back(entry);
                }

                std::uint32_t SomeIpSdMessage::Length() const noexcept
                {
                    const std::uint32_t cLengthFieldSize = 4;
                    // Request ID + Versions + Message Type + Return Code
                    const std::uint32_t cGeneralHeaderSize = 8;
                    // Flags + Reserved
                    const std::uint32_t cSdHeaderSize = 4;

                    uint32_t _entriesLength = cLengthFieldSize + getEntriesLength();
                    uint32_t _optionsLength = cLengthFieldSize + getOptionsLength();
                    uint32_t _result =
                        cGeneralHeaderSize +
                        cSdHeaderSize +
                        _entriesLength +
                        _optionsLength;

                    return _result;
                }

                void SomeIpSdMessage::SetSessionId(std::uint16_t sessionId)
                {
                    if (sessionId == 0)
                    {
                        throw std::invalid_argument(
                            "Session ID cannot be zero for the SD message.");
                    }

                    SomeIpMessage::SetSessionId(sessionId);
                }

                bool SomeIpSdMessage::IncrementSessionId() noexcept
                {
                    bool _wrapped = SomeIpMessage::IncrementSessionId();
                    // If the message is rebooted, check for wrapping
                    mRebooted &= !_wrapped;
                }

                const std::vector<std::uint8_t> &SomeIpSdMessage::Payload()
                {
                    // General SOME/IP header payload insertion
                    std::vector<std::uint8_t> _result = SomeIpMessage::Payload();

                    if (mRebooted)
                    {
                        // Both Unicast Support and Explicit Initial Data Control flags are on.
                        const std::uint32_t cRebootedFlag = 0xE000;
                        helper::Inject(_result, cRebootedFlag);
                    }
                    else
                    {
                        // Both Unicast Support and Explicit Initial Data Control flags are on.
                        const std::uint32_t cNotRebootedFlag = 0x6000;
                        helper::Inject(_result, cNotRebootedFlag);
                    }

                    std::uint8_t _lastOptionIndex = 0;
                    std::vector<std::uint8_t> _entriesPayload;
                    std::vector<std::uint8_t> _optionsPayload;
                    for (auto entry : Entries())
                    {
                        auto _entryPayload = entry.Payload(_lastOptionIndex);
                        helper::Concat(
                            _entriesPayload, std::move(_entryPayload));

                        for (auto firstOption : entry.FirstOptions())
                        {
                            auto _firstOptionPayload = firstOption->Payload();
                            helper::Concat(
                                _optionsPayload, std::move(_firstOptionPayload));
                            ++_lastOptionIndex;
                        }

                        for (auto secondOption : entry.SecondOptions())
                        {
                            auto _secondOptionPayload = secondOption->Payload();
                            helper::Concat(
                                _optionsPayload, std::move(_secondOptionPayload));
                            ++_lastOptionIndex;
                        }
                    }

                    // Entries length and payloads insertion
                    std::uint32_t _entriesLength = getEntriesLength();
                    helper::Inject(_result, _entriesLength);
                    helper::Concat(_result, std::move(_entriesPayload));

                    // Options length and payloads insertion
                    std::uint32_t _optionsLength = getOptionsLength();
                    helper::Inject(_result, _optionsLength);
                    helper::Concat(_result, std::move(_optionsPayload));

                    return _result;
                }
            }
        }
    }
}