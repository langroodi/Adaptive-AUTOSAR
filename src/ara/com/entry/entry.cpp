#include "./entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            Entry::Entry(EntryType type,
                         uint16_t serviceId,
                         uint16_t instanceId,
                         uint32_t ttl,
                         uint8_t majorVersion) noexcept : mType{type},
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

            uint16_t Entry::ServiceId() const noexcept
            {
                return mServiceId;
            }

            uint16_t Entry::InstanceId() const noexcept
            {
                return mInstanceId;
            }

            uint8_t Entry::MajorVersion() const noexcept
            {
                return mMajorVersion;
            }

            uint32_t Entry::TTL() const noexcept
            {
                return mTTL;
            }

            std::vector<option::Option*> Entry::FirstOptions() const noexcept
            {
                return mFirstOptions;
            }

            void Entry::AddFirstOption(option::Option* firstOption)
            {
                mFirstOptions.push_back(firstOption);
            }

            std::vector<option::Option*> Entry::SecondOptions() const noexcept
            {
                return mSecondOptions;
            }

            void Entry::AddSecondOption(option::Option* secondOption)
            {
                mSecondOptions.push_back(secondOption);
            }

            std::vector<uint8_t> Entry::Payload(uint8_t &optionIndex)
            {
                std::vector<uint8_t> _result;

                uint8_t _type = static_cast<uint8_t>(Type());
                _result.push_back(_type);

                _result.push_back(optionIndex);
                uint8_t _firstOptionsSize = FirstOptions().size();
                optionIndex += _firstOptionsSize;

                _result.push_back(optionIndex);
                uint8_t _secondOptionsSize = SecondOptions().size();
                optionIndex += _secondOptionsSize;

                const uint8_t cOptionSizeBitLength = 4;
                _firstOptionsSize <<= cOptionSizeBitLength;
                _firstOptionsSize |= _secondOptionsSize;
                _result.push_back(_firstOptionsSize);

                helper::Inject(_result, ServiceId());
                helper::Inject(_result, InstanceId());

                const uint8_t cTTLSizeBitLength = 24;
                uint32_t _majorVersion = MajorVersion();
                _majorVersion <<= cTTLSizeBitLength;
                _majorVersion |= TTL();
                helper::Inject(_result, _majorVersion);

                return _result;
            }

            Entry::~Entry() noexcept
            {
                for (auto firstOption : mFirstOptions)
                {
                    delete firstOption;
                }

                for (auto secondOption : mSecondOptions)
                {
                    delete secondOption;
                }
            }
        }
    }
}