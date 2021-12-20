#ifndef ERROR_DOMAIN_H
#define ERROR_DOMAIN_H

#include <stdint.h>

namespace ara
{
    /// @brief ARA basic core types namespace
    namespace core
    {
        /// @brief A class that defines the domain of an ErrorCode to avoid code interferences
        /// @note The class is literal type and it is recommended that derived classes be literal type as well.
        class ErrorDomain
        {
        public:
            /// @brief Alias type of the domain ID
            using IdType = uint64_t;
            /// @brief Alias type of the error code
            using CodeType = uint32_t;

        private:
            IdType mId;

        public:
            /// @brief Constructor
            /// @param id Error domain ID
            explicit constexpr ErrorDomain(IdType id) noexcept : mId{id}
            {
            }

            ~ErrorDomain() noexcept = default;

            ErrorDomain(const ErrorDomain &) = delete;
            ErrorDomain(ErrorDomain &&) = delete;
            ErrorDomain &operator=(const ErrorDomain &) = delete;
            ErrorDomain &operator=(ErrorDomain &&) = delete;

            constexpr bool operator==(const ErrorDomain &other) const noexcept
            {
                return mId == other.mId;
            }

            constexpr bool operator!=(const ErrorDomain &other) const noexcept
            {
                return mId != other.mId;
            }

            /// @brief Get the domain ID
            /// @returns Error domain ID
            constexpr IdType Id() const noexcept
            {
                return mId;
            }

            /// @brief Get the domain's name
            /// @returns Error domain name
            virtual const char *Name() const noexcept = 0;

            /// @brief Get error message of a specific error code
            /// @param errorCode Error code of interest
            /// @returns Error code message in this domain
            virtual const char *Message(CodeType errorCode) const noexcept = 0;
        };
    }
}

#endif