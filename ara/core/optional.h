#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <type_traits>

namespace ara
{
    namespace core
    {
        template <typename T>
        class Optional
        {
        private:
            T mValue;
            bool mHasValue;

        public:
            constexpr Optional() noexcept;
            constexpr Optional(T &&value) noexcept(
                std::is_nothrow_move_constructible<T>::value);
            ~Optional();
            constexpr Optional &operator=(const Optional &other);
            constexpr Optional &operator=(Optional &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value);
            constexpr const T &operator*() const &;
            constexpr T &operator*() &;
            constexpr explicit operator bool() const noexcept;
            constexpr bool has_value() const noexcept;
            constexpr const T &value() const &;
            constexpr T &value() &;
            constexpr T value_or(T &&defaultValue) const &;
            void reset() noexcept;
        };
    }
}
#endif