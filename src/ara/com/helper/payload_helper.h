#ifndef PAYLOAD_HELPER_H
#define PAYLOAD_HELPER_H

#include <vector>
#include <stdint.h>

namespace ara
{
    namespace com
    {
        /// @brief Helper functions required for communication classes
        /// @note The namespace is not part of the official AUTOSAR standard.
        namespace helper
        {
            /// @brief Inject a short value into a byte vector
            /// @param vector Byte vector
            /// @param value Short input value
            void Inject(std::vector<uint8_t> &vector, uint16_t value);

            /// @brief Inject an integer value into a byte vector
            /// @param vector Byte vector
            /// @param value Integer input value
            void Inject(std::vector<uint8_t> &vector, uint32_t value);

            /// @brief Concat the second vector into the end of the first vector
            /// @param vector1 First vector
            /// @param vector2 Second vector
            void Concat(std::vector<uint8_t> &vector1, std::vector<uint8_t> &&vector2);

            /// @brief Extract a short value from a byte vector
            /// @param vector Byte vector
            /// @param offset Extract offset at the vector
            /// @returns Extracted short value
            uint16_t ExtractShort(
                const std::vector<uint8_t> &vector, std::size_t &offset);

            /// @brief Extract an integer value from a byte vector
            /// @param vector Byte vector
            /// @param offset Extract offset at the vector
            /// @returns Extracted integer value
            uint32_t ExtractInteger(
                const std::vector<uint8_t> &vector, std::size_t &offset);
        }
    }
}
#endif