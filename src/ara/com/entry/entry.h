#ifndef ENTRY_H
#define ENTRY_H

#include <stdint.h>
#include <vector>
#include "../option/option.h"

namespace ara
{
    namespace com
    {
        /// @brief Communication message entry
        namespace entry
        {
            /// @brief Message entry type
            enum class EntryType : std::uint8_t
            {
                Finding = 0x00,      ///!< Service finding
                Offering = 0x01,     ///!< Service start/stop offering
                Subscribing = 0x06,  ///!< Event start/stop subscribing
                Acknowledging = 0x07 ///!< Event subscribe positive/negative acknowledging
            };

            /// @brief Communication message abstract entry
            class Entry
            {
            private:
                EntryType mType;
                std::vector<option::Option> mFirstOptions;
                std::vector<option::Option> mSecondOptions;
                std::uint16_t mServiceId;
                std::uint16_t mInstanceId;
                uint8_t mMajorVersion;
                uint32_t mTTL;

            protected:
                /// @brief Constructor
                /// @param type Entry type
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param ttl Entry time to live
                /// @param majorVersion Service in interest major version
                Entry(EntryType type,
                      std::uint16_t serviceId,
                      std::uint16_t instanceId,
                      uint32_t ttl,
                      uint8_t majorVersion = 0xff);

            public:
                virtual ~Entry() noexcept = default;

                /// @brief Get entry type
                /// @returns Entry type
                EntryType Type() const noexcept;

                /// @brief Get service ID
                /// @returns Entry service in interest ID
                std::int16_t ServiceId() const noexcept;

                /// @brief Get instance ID
                /// @returns Entry service in interest instance ID
                std::uint16_t InstanceId() const noexcept;

                /// @brief Get major version
                /// @returns Entry service in interest major version
                std::uint8_t MajorVersion() const noexcept;

                /// @brief Get TTL
                /// @returns Entry time to live in seconds
                std::uint32_t TTL() const noexcept;

                /// @brief Get first (general) options
                /// @returns Exisiting first options
                const std::vector<option::Option> &FirstOptions() const noexcept;

                /// @brief Add a first (general) option
                /// @param firstOption First option to be added
                void AddFirstOption(option::Option &&firstOption);

                /// @brief Get second (specific) options
                /// @returns Exisiting second options
                const std::vector<option::Option> &SecondOptions() const noexcept;

                /// @brief Add a second (specific) option
                /// @param secondOption Second option to be added
                void AddSecondOption(option::Option &&secondOption);

                /// @brief Get entity payload
                /// @returns Byte array
                virtual const std::vector<std::uint8_t> &Payload() = 0;
            };
        }
    }
}

#endif