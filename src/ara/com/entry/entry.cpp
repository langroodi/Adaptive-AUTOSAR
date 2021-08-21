#include "./entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            Entry::Entry(EntryType type,
                         std::uint16_t serviceId,
                         std::uint16_t instanceId,
                         std::uint32_t ttl,
                         std::uint8_t majorVersion) noexcept : mType{type},
                                                               mServiceId{serviceId},
                                                               mInstanceId{instanceId},
                                                               mTTL{ttl},
                                                               mMajorVersion{majorVersion}
            {
            }

            EntryType Entry::Type() const noexcept
            {
                return mType;
            }

            std::int16_t Entry::ServiceId() const noexcept
            {
                return mServiceId;
            }

            std::uint16_t Entry::InstanceId() const noexcept
            {
                return mInstanceId;
            }

            std::uint8_t Entry::MajorVersion() const noexcept
            {
                return mMajorVersion;
            }

            std::uint32_t Entry::TTL() const noexcept
            {
                return mTTL;
            }

            const std::vector<option::Option> &Entry::FirstOptions() const noexcept
            {
                return mFirstOptions;
            }

            void Entry::AddFirstOption(option::Option &&firstOption)
            {
                mFirstOptions.push_back(firstOption);
            }

            const std::vector<option::Option> &Entry::SecondOptions() const noexcept
            {
                return mSecondOptions;
            }

            void Entry::AddSecondOption(option::Option &&secondOption)
            {
                mSecondOptions.push_back(secondOption);
            }

            const std::vector<std::uint8_t> &Entry::Payload(std::uint8_t &optionIndex)
            {
                std::vector<std::uint8_t> _result;

                std::uint8_t _type = (std::uint8_t)Type();
                _result.push_back(_type);

                _result.push_back(optionIndex);
                std::uint8_t _firstOptionsSize = FirstOptions().size();
                optionIndex += _firstOptionsSize;

                _result.push_back(optionIndex);
                std::uint8_t _secondOptionsSize = SecondOptions().size();
                optionIndex += _secondOptionsSize;

                const std::uint8_t cOptionSizeBitLength = 4;
                _firstOptionsSize << cOptionSizeBitLength;
                _firstOptionsSize |= _secondOptionsSize;
                _result.push_back(_firstOptionsSize);

                someip::SomeIpMessage::Inject(_result, ServiceId());
                someip::SomeIpMessage::Inject(_result, InstanceId());

                const std::uint8_t cTTLSizeBitLength = 24;
                std::uint32_t _majorVersion = MajorVersion();
                _majorVersion << cTTLSizeBitLength;
                _majorVersion |= TTL();
                _result.push_back(_majorVersion);

                return _result;
            }
        }
    }
}