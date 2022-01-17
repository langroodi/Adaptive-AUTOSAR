#ifndef OPTION_H
#define OPTION_H

#include <stdint.h>
#include <vector>
#include "../helper/payload_helper.h"

namespace ara
{
    namespace com
    {
        /// @brief Communication message entry option
        namespace option
        {
            /// @brief Entry option type
            enum class OptionType : uint8_t
            {
                Configuration = 0x01,  ///< DNS configuration
                LoadBalancing = 0x02,  ///< Service instnace load balancing
                IPv4Endpoint = 0x04,   ///< Generic IPv4 unicast endpoint
                IPv6Endpoint = 0x06,   ///< Generic IPv6 unicast endpoint
                IPv4Multicast = 0x14,  ///< Generic IPv4 multicast endpoint
                IPv6Multicast = 0x16,  ///< Genetic IPv6 multicast endpoint
                IPv4SdEndpoint = 0x24, ///< Service discovery IPv4 endpoint
                IPv6SdEndpoint = 0x26  ///< Service discovery IPv6 endpoint
            };

            /// @brief OSI layer-4 protocol type
            enum class Layer4ProtocolType : uint8_t
            {
                Tcp = 0x06, ///< Transmission control protocol
                Udp = 0x11  ///< User datagram protocol
            };

            /// @brief Communication message entry abstract option
            class Option
            {
            private:
                OptionType mType;
                bool mDiscardable;

            protected:
                /// @brief Constructor
                /// @param type Option type
                /// @param discardable Indicates whether the option can be discarded or not
                constexpr Option(OptionType type, bool discardable) noexcept : mType{type},
                                                                               mDiscardable{discardable}
                {
                }

                /// @brief Get base option payload
                /// @returns Byte array
                std::vector<uint8_t> BasePayload() const;

            public:
                virtual ~Option() noexcept = default;

                /// @brief Get option length
                /// @returns Option length in bytes
                virtual uint16_t Length() const noexcept = 0;

                /// @brief Get option type
                /// @returns Option type
                OptionType Type() const noexcept;

                /// @brief Get discardable flag
                /// @returns True if the option can be discarded; otherwise false
                bool Discardable() const noexcept;

                /// @brief Get option payload
                /// @returns Byte array
                virtual std::vector<uint8_t> Payload() const = 0;
            };
        }
    }
}

#endif