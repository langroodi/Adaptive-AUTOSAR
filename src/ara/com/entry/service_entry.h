#ifndef SERVICE_ENTRY_H
#define SERVICE_ENTRY_H

#include <stdint.h>
#include <stdexcept>
#include "./entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            /// @brief Entry to find and offer a service
            class ServiceEntry : public Entry
            {
            private:
                const std::uint16_t cAnyInstanceId = 0xffff;
                const std::uint32_t cAnyMinorVersion = 0xffffffff;

                std::uint32_t mMinorVersion;

                /// @brief Constructor
                /// @param type Entry type
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param ttl Entry time to live
                /// @param majorVersion Service in interest major version
                /// @param minorVersion Service in interest minor version
                ServiceEntry(EntryType type,
                             std::uint16_t serviceId,
                             std::uint16_t instanceId,
                             std::uint32_t ttl,
                             sts::uint8_t majorVersion,
                             std::uint32_t minorVersion) noexcept;

            public:
                ServiceEntry() = delete;

                /// @brief Get minor version
                /// @returns Service minor version
                std::uint32_t MinorVersion() const noexcept;

                /// @brief Find a service entry factory
                /// @param serviceId Service in interest ID
                /// @param ttl Entry time to live
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param minorVersion Service in interest minor version
                /// @returns Find service entry
                static ServiceEntry CreateFindServiceEntry(
                    std::uint16_t serviceId,
                    std::uint32_t ttl,
                    std::uint16_t instanceId = cAnyInstanceId,
                    sts::uint8_t majorVersion = cAnyMajorVersion,
                    std::uint32_t minorVersion = cAnyMinorVersion) noexcept;

                /// @brief Offer a service entry factory
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param minorVersion Service in interest minor version
                /// @returns Offer service entry
                static ServiceEntry CreateOfferServiceEntry(
                    std::uint16_t serviceId,
                    std::uint16_t instanceId,
                    sts::uint8_t majorVersion,
                    std::uint32_t minorVersion) noexcept;

                /// @brief Stop offer a service entry factory
                /// @param serviceId Service in interest ID
                /// @param instanceId Service in interest instance ID
                /// @param majorVersion Service in interest major version
                /// @param minorVersion Service in interest minor version
                /// @returns Stop service offering entry
                static ServiceEntry CreateStopOfferEntry(
                    std::uint16_t serviceId,
                    std::uint16_t instanceId,
                    sts::uint8_t majorVersion,
                    std::uint32_t minorVersion) noexcept;
            };
        }
    }
}

#endif