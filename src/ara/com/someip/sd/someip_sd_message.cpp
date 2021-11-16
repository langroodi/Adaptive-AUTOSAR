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

                uint32_t SomeIpSdMessage::getEntriesLength() const noexcept
                {
                    const uint32_t cEntrySize = 16;
                    uint32_t _numberOfEntries = mEntries.size();
                    uint32_t _result = _numberOfEntries * cEntrySize;

                    return _result;
                }

                uint32_t SomeIpSdMessage::getOptionsLength() const noexcept
                {
                    const uint32_t cOptionLengthFieldSize = 3;
                    uint32_t _result = 0;

                    for (auto entry : mEntries)
                    {
                        for (auto firstOption : entry->FirstOptions())
                        {
                            _result += cOptionLengthFieldSize + firstOption->Length();
                        }

                        for (auto secondOption : entry->SecondOptions())
                        {
                            _result += cOptionLengthFieldSize + secondOption->Length();
                        }
                    }

                    return _result;
                }

                const std::vector<std::shared_ptr<entry::Entry>> &SomeIpSdMessage::Entries() const noexcept
                {
                    return mEntries;
                }

                void SomeIpSdMessage::AddEntry(std::shared_ptr<entry::Entry> entry)
                {
                    mEntries.push_back(entry);
                }

                uint32_t SomeIpSdMessage::Length() const noexcept
                {
                    const uint32_t cLengthFieldSize = 4;
                    // Request ID + Versions + Message Type + Return Code
                    const uint32_t cGeneralHeaderSize = 8;
                    // Flags + Reserved
                    const uint32_t cSdHeaderSize = 4;

                    uint32_t _entriesLength = cLengthFieldSize + getEntriesLength();
                    uint32_t _optionsLength = cLengthFieldSize + getOptionsLength();
                    uint32_t _result =
                        cGeneralHeaderSize +
                        cSdHeaderSize +
                        _entriesLength +
                        _optionsLength;

                    return _result;
                }

                void SomeIpSdMessage::SetSessionId(uint16_t sessionId)
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

                    return _wrapped;
                }

                std::vector<uint8_t> SomeIpSdMessage::Payload() const
                {
                    // General SOME/IP header payload insertion
                    std::vector<uint8_t> _result = SomeIpMessage::Payload();

                    if (mRebooted)
                    {
                        // Both Unicast Support and Explicit Initial Data Control flags are on.
                        helper::Inject(_result, cRebootedFlag);
                    }
                    else
                    {
                        // Both Unicast Support and Explicit Initial Data Control flags are on.
                        helper::Inject(_result, cNotRebootedFlag);
                    }

                    uint8_t _lastOptionIndex = 0;
                    std::vector<uint8_t> _entriesPayload;
                    std::vector<uint8_t> _optionsPayload;
                    for (auto entry : mEntries)
                    {
                        auto _entryPayload = entry->Payload(_lastOptionIndex);
                        helper::Concat(
                            _entriesPayload, std::move(_entryPayload));

                        for (auto firstOption : entry->FirstOptions())
                        {
                            auto _firstOptionPayload = firstOption->Payload();
                            helper::Concat(
                                _optionsPayload, std::move(_firstOptionPayload));
                            ++_lastOptionIndex;
                        }

                        for (auto secondOption : entry->SecondOptions())
                        {
                            auto _secondOptionPayload = secondOption->Payload();
                            helper::Concat(
                                _optionsPayload, std::move(_secondOptionPayload));
                            ++_lastOptionIndex;
                        }
                    }

                    // Entries length and payloads insertion
                    uint32_t _entriesLength = getEntriesLength();
                    helper::Inject(_result, _entriesLength);
                    helper::Concat(_result, std::move(_entriesPayload));

                    // Options length and payloads insertion
                    uint32_t _optionsLength = getOptionsLength();
                    helper::Inject(_result, _optionsLength);
                    helper::Concat(_result, std::move(_optionsPayload));

                    return _result;
                }

                SomeIpSdMessage SomeIpSdMessage::Deserialize(
                    const std::vector<uint8_t> &payload)
                {
                    SomeIpSdMessage _result;
                    SomeIpMessage::Deserialize(&_result, payload);

                    std::size_t _someIpSdPduOffset = 16;
                    uint32_t _rebootFlag =
                        helper::ExtractInteger(payload, _someIpSdPduOffset);
                    if (_rebootFlag == cRebootedFlag)
                    {
                        _result.mRebooted = true;
                    }
                    else if (_rebootFlag == cNotRebootedFlag)
                    {
                        _result.mRebooted = false;
                    }
                    else
                    {
                        throw std::out_of_range(
                            "The serialized reboot flag is out of range.");
                    }

                    return _result;
                }
            }
        }
    }
}