#ifndef ATOMIC_OPTIONAL_H
#define ATOMIC_OPTIONAL_H

#include <type_traits>
#include <utility>
#include <stdexcept>
#include <atomic>

namespace ara
{
    namespace exec
    {
        /// @brief Unofficial namespace for Execution Management helper classes
        namespace helper
        {
            /// @brief A wrapper around a possible atomic value
            /// @tparam T Possible non-atomic value type
            template <typename T>
            class AtomicOptional
            {
            private:
                std::atomic<T> mValue;
                std::atomic<bool> mHasValue;

            public:
                AtomicOptional() noexcept : mHasValue{false}
                {
                }

                /// @brief Constructor
                /// @param value Desired non-atomic value
                constexpr AtomicOptional(T value) noexcept : mValue{value},
                                                             mHasValue{true}
                {
                }

                ~AtomicOptional() noexcept = default;
                AtomicOptional &operator=(const AtomicOptional& other) = delete;

                AtomicOptional &operator=(T value) noexcept
                {
                    mValue = value;
                    mHasValue = true;

                    return *this;
                }

                /// @brief Reset the instance atomic value
                void Reset() noexcept
                {
                    mHasValue = false;
                }

                /// @brief Indicate whether the instance has an atomic value or not
                /// @returns True if has the value, otherwise false
                bool HasValue() const noexcept
                {
                    return mHasValue;
                }

                /// @brief Get instance possible non-atomic value
                /// @returns Copied non-atomic value
                /// @throws std::runtime_error Throws if there is no value
                T Value() const
                {
                    if (mHasValue)
                    {
                        return mValue.load();
                    }
                    else
                    {
                        throw std::runtime_error("Optional contains no value.");
                    }
                }
            };
        }
    }
}

#endif