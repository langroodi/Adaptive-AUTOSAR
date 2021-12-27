#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <type_traits>
#include <utility>
#include <stdexcept>

namespace ara
{
    namespace core
    {
        /// @brief A wrapper around a possible value
        /// @tparam T Possible value type
        template <typename T>
        class Optional
        {
        private:
            T *mValuePtr;
            bool mHasValue;

        public:
            constexpr Optional() noexcept : mHasValue{false}
            {
            }

            Optional(const T &value)
            {
                mValuePtr = new T{value};
                mHasValue = true;
            }

            Optional(T &&value)
            {
                mValuePtr = new T{value};
                mHasValue = true;
            }

            Optional(const Optional &other)
            {
                if (other.mHasValue)
                {
                    mValuePtr = new T{*other.mValuePtr};
                }

                mHasValue = other.mHasValue;
            }

            Optional(Optional &&other) noexcept(
                std::is_nothrow_move_assignable<T>::value)
            {
                if (other.mHasValue)
                {
                    mValuePtr = new T{std::move(*other.mValuePtr)};
                    mHasValue = true;
                    other.mHasValue = false;
                }
            }

            ~Optional()
            {
                Reset();
            }

            constexpr Optional &operator=(const Optional &other)
            {
                Reset();

                if (other.mHasValue)
                {
                    mValuePtr = new T{*other.mValue};
                }

                mHasValue = other.mHasValue;

                return *this;
            }

            constexpr Optional &operator=(Optional &&other) noexcept(
                std::is_nothrow_move_assignable<T>::value)
            {
                Reset();

                if (other.mHasValue)
                {
                    mValuePtr = new T{std::move(*other.mValuePtr)};
                    mHasValue = true;
                    other.mHasValue = false;
                }

                return *this;
            }

            template <typename U = T>
            constexpr Optional &operator=(const U &value)
            {
                Reset();
                mValuePtr = new T{static_cast<const T &>(value)};
                mHasValue = true;

                return *this;
            }

            template <typename U = T>
            constexpr Optional &operator=(U &&value)
            {
                Reset();
                mValuePtr = new T{static_cast<T &&>(value)};
                mHasValue = true;

                return *this;
            }

            /// @brief Construct a new value from the give argument(s) and assign it to the instance value
            /// @param args Argument(s) to construct a new value
            template <typename... Args>
            void Emplace(Args &&...args)
            {
                Reset();
                mValuePtr = new T{args...};
                mHasValue = true;
            }

            /// @brief Swap the current instance with another one
            /// @param other Another Optional for swapping
            void Swap(Optional &other) noexcept(
                std::is_nothrow_move_assignable<T>::value)
            {
                if (mHasValue && other.mHasValue)
                {
                    std::swap(mValuePtr, other.mValuePtr);
                }
                else if (mHasValue && !other.mHasValue)
                {
                    other.mValuePtr = mValuePtr;
                    other.mHasValue = true;
                    mHasValue = false;
                }
                else if (!mHasValue && other.mHasValue)
                {
                    mValuePtr = other.mValuePtr;
                    mHasValue = true;
                    other.mHasValue = false;
                }
            }

            /// @brief Reset the instance value
            void Reset() noexcept
            {
                if (mHasValue)
                {
                    delete mValuePtr;
                    mHasValue = false;
                }
            }

            /// @brief Indicate whether the instance has a value or not
            /// @returns True if has a value, otherwise false
            constexpr bool HasValue() const noexcept
            {
                return mHasValue;
            }

            /// @returns True if the instance has a value, otherwise false
            constexpr explicit operator bool() const noexcept
            {
                return mHasValue;
            }

            /// @returns Copied value
            /// @throws std::runtime_error Throws if there is no value
            const T &operator*() const &
            {
                if (mHasValue)
                {
                    return *mValuePtr;
                }
                else
                {
                    throw std::runtime_error("Optional contains no value.");
                }
            }

            /// @returns Movable value
            /// @throws std::runtime_error Throws if there is no value
            T &&operator*() &&
            {
                if (mHasValue)
                {
                    return std::move(*mValuePtr);
                }
                else
                {
                    throw std::runtime_error("Optional contains no value.");
                }
            }

            /// @returns Constant value pointer
            /// @throws std::runtime_error Throws if there is no value
            const T *operator->() const
            {
                if (mHasValue)
                {
                    mValuePtr;
                }
                else
                {
                    throw std::runtime_error("Optional contains no value.");
                }
            }

            /// @brief Get instance possible value
            /// @returns Copied value
            /// @throws std::runtime_error Throws if there is no value
            const T &Value() const &
            {
                if (mHasValue)
                {
                    return *mValuePtr;
                }
                else
                {
                    throw std::runtime_error("Optional contains no value.");
                }
            }

            /// @brief Get instance possible value
            /// @returns Movable value
            /// @throws std::runtime_error Throws if there is no value
            T &&Value() &&
            {
                if (mHasValue)
                {
                    mHasValue = false;
                    return std::move(*mValuePtr);
                }
                else
                {
                    throw std::runtime_error("Optional contains no value.");
                }
            }

            /// @brief Get the instance value or the default value
            /// @tparam U Default value type
            /// @param defaultValue Value to be return in absence of the instance value
            /// @returns Instance value if exists, otherwise the default value
            template <typename U>
            T ValueOr(U &&defaultValue) const &
            {
                if (mHasValue)
                {
                    return *mValuePtr;
                }
                else
                {
                    return static_cast<T>(defaultValue);
                }
            }

            /// @brief Get the instance value or the default value
            /// @tparam U Default value type
            /// @param defaultValue Value to be return in absence of the instance value
            /// @returns Instance value if exists, otherwise the default value
            template <typename U>
            T ValueOr(U &&defaultValue) &&
            {
                if (mHasValue)
                {
                    return std::move(*mValuePtr);
                }
                else
                {
                    return static_cast<T>(defaultValue);
                }
            }
        };

        /// @returns True if both optional instances contain a value and their values are equal or both do not contain a value, otherwise false
        template <typename T>
        inline bool operator==(const Optional<T> &lhs, const Optional<T> &rhs) noexcept
        {
            bool _result;

            if (lhs.HasValue() && rhs.HasValue())
            {
                _result = lhs.Value() == rhs.Value();
            }
            else if (!lhs.HasValue() && !rhs.HasValue())
            {
                _result = true;
            }
            else
            {
                _result = false;
            }

            return _result;
        }

        /// @returns False if both optional instances contain a value and their values are equal or both do not contain a value, otherwise true
        template <typename T>
        inline bool operator!=(const Optional<T> &lhs, const Optional<T> &rhs) noexcept
        {
            bool _result;

            if (lhs.HasValue() && rhs.HasValue())
            {
                _result = lhs.Value() != rhs.Value();
            }
            else if (!lhs.HasValue() && !rhs.HasValue())
            {
                _result = false;
            }
            else
            {
                _result = true;
            }

            return _result;
        }
    }
}

#endif