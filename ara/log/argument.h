#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <type_traits>

namespace ara
{
    namespace log
    {
        template <typename T>
        class Argument
        {
        private:
            T mPayload;
            std::string mIdentifier;
            std::string mUnit;

        public:
            constexpr Argument(
                T &&t,
                const char *indetifier,
                const char *unit) noexcept(std::is_nothrow_move_constructible<T>::value);
            ~Argument(std::i) noexcept;
        };
    }
}

#endif