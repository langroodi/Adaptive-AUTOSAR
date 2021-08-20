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
                return mType();
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
        }
    }
}