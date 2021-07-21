#ifndef ERROR_DOMAIN_H
#define ERROR_DOMAIN_H

#include <cstdint>

namespace ara
{
    namespace core
    {
        class ErrorDomain
        {
        public:
            using IdType = std::uint64_t;
            using CodeType = std::uint32_t;

            ErrorDomain(const ErrorDomain &) = delete;
            ErrorDomain(ErrorDomain &&) = delete;
            explicit constexpr ErrorDomain(IdType id) noexcept;
            ~ErrorDomain() = default;
            ErrorDomain &operator=(const ErrorDomain &) = delete;
            ErrorDomain &operator=(ErrorDomain &&) = delete;
            constexpr bool operator==(const ErrorDomain &other) const noexcept;
            constexpr bool operator!=(const ErrorDomain &other) const noexcept;
            constexpr IdType Id() const noexcept;
            virtual const char *Name() const noexcept = 0;
            virtual const char *Message(CodeType errorCode) const noexcept = 0;
        };
    }
}

#endif