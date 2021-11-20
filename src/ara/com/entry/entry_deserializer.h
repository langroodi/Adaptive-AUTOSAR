#ifndef ENTRY_DESERIALIZER_H
#define ENTRY_DESERIALIZER_H

#include "./eventgroup_entry.h"
#include "./service_entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            /// @brief A helper class for deserialzing an entry payload
            class EntryDeserializer
            {
            private:
                std::shared_ptr<Entry> mDeserializedEntry;
                uint8_t mNumberOfFirstOptions;
                uint8_t mNumberOfSecondOptions;

            public:
                /// @brief Constructor
                /// @param payload Serialized entry payload byte array
                /// @param offset Deserializing offset in the payload
                /// @returns Shared pointer to the entry which is created while deserializing
                /// @throws std::out_of_range Throws when the entry type for deserializing is not supported
                EntryDeserializer(
                    const std::vector<uint8_t> &payload,
                    std::size_t &offset);

                EntryDeserializer() = default;
                ~EntryDeserializer() noexcept = default;

                /// @brief Get the deserialized entry while construction
                /// @returns Shared pointer to the deserialized entry
                std::shared_ptr<Entry> DeserializedEntry() const noexcept;

                /// @brief Get the number of first options that the deserialized entry have
                /// @returns A 4-bit number wrapped as a byte
                uint8_t NumberOfFirstOptions() const noexcept;

                /// @brief Get the number of first options that the deserialized entry have
                /// @returns A 4-bit number wrapped as a byte
                uint8_t NumberOfSecondOptions() const noexcept;
            };
        }
    }
}

#endif