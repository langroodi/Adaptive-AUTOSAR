#ifndef RESULT_H
#define RESULT_H

#include <type_traits>
#include <utility>
#include <stdexcept>
#include "./error_code.h"

namespace ara
{
    namespace core
    {
        /// @brief A wrapper around the callee's return value and its possible error
        /// @tparam T Callee's return value type
        /// @tparam E Callee's possible error type
        template <typename T, typename E = ErrorCode>
        class Result final
        {
        private:
            T mValue;
            E mError;
            bool mHasValue;

            static void copy(Result *source, Result *destination)
            {
                // Copy the value if exists, otherwise copy the error
                if (source->HasValue)
                {
                    destination->mValue = source->mValue;
                }
                else
                {
                    destination->mError = source->mError;
                }

                destination->mHasValue = source->HasValue;
            }

            static void move(Result *source, Result *destination)
            {
                // Move the value if exists, otherwise move the error
                if (source->HasValue)
                {
                    destination->mValue = std::move(source->mValue);
                }
                else
                {
                    destination->mError = std::move(source->mError);
                }

                destination->mHasValue = source->HasValue;
            }

        public:
            /// @brief Result value type alias
            using value_type = T;
            /// @brief Result error type alias
            using error_type = E;

            Result(const T &t) : mValue{t},
                                 mHasValue{true}
            {
            }

            Result(T &&t) : mValue{std::move(t)},
                            mHasValue{false}
            {
            }

            explicit Result(const E &e) : mError{e},
                                          // Result cannot contain both value and error at the same time.
                                          mHasValue{false}
            {
            }

            explicit Result(E &&e) : mError{std::move(e)},
                                     // Result cannot contain both value and error at the same time.
                                     mHasValue{false}
            {
            }

            Result(const Result &other)
            {
                copy(&other, this);
            }

            Result(Result &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&
                    std::is_nothrow_move_constructible<E>::value)
            {
                move(&other, this);
            }

            ~Result() noexcept = default;

            /// @brief Result factory by copying its value
            /// @param t Result value
            static Result FromValue(const T &t)
            {
                Result _result{t};
                return _result;
            }

            /// @brief Result factory by moving its value
            /// @param t Result value
            static Result FromValue(T &&t)
            {
                Result _result{std::move(t)};
                return _result;
            }

            /// @brief Result factory by copying its error
            /// @param e Result error
            static Result FromError(const E &e)
            {
                Result _result{e};
                return _result;
            }

            /// @brief Result factory by moving its error
            /// @param e Result error
            static Result FromError(E &&e)
            {
                Result _result{std::move(e)};
                return _result;
            }

            Result &operator=(Result const &other)
            {
                copy(&other, this);
            }

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&
                    std::is_nothrow_move_assignable<T>::value &&
                        std::is_nothrow_move_constructible<E>::value &&
                            std::is_nothrow_move_assignable<E>::value)
            {
                move(&other, this);
            }

            /// @brief Construct a new value from the give argument(s) and assign it to the instance value
            /// @param args Argument(s) to construct a new value
            template <typename... Args>
            void EmplaceValue(Args &&...args)
            {
                mValue = T{args...};
                mHasValue = true;
            }

            /// @brief Construct a new error from the give argument(s) and assign it to the instance error
            /// @param args Argument(s) to construct a new error
            template <typename... Args>
            void EmplaceError(Args &&...args)
            {
                mError = T{args...};
                mHasValue = false;
            }

            void Swap(Result &other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&
                    std::is_nothrow_move_assignable<T>::value &&
                        std::is_nothrow_move_constructible<E>::value &&
                            std::is_nothrow_move_assignable<E>::value)
            {
                std::swap(*this, other);
            }

            /// @brief Indicate whether the instance has a value or not
            /// @returns True if has a value and false if it contains an error
            bool HasValue() const noexcept
            {
                return mHasValue;
            }

            /// @returns True if has a value and false if it contains an error
            explicit operator bool() const noexcept
            {
                return mHasValue;
            }

            /// @returns Copied value
            /// @throws std::logic_error Throws if there is no value
            const T &operator*() const &
            {
                if (mHasValue)
                {
                    return mValue;
                }
                else
                {
                    throw std::logic_error("Result contains no value.");
                }
            }

            /// @returns Movable value
            /// @throws std::logic_error Throws if there is no value
            T &&operator*() &&
            {
                if (mHasValue)
                {
                    return std::move(mValue);
                }
                else
                {
                    throw std::logic_error("Result contains no value.");
                }
            }

            /// @returns Constant value pointer
            /// @throws std::logic_error Throws if there is no value
            const T *operator->() const
            {
                if (mHasValue)
                {
                    &mValue;
                }
                else
                {
                    throw std::logic_error("Result contains no value.");
                }
            }

            /// @brief Get Result possible value
            /// @returns Copied value
            /// @throws std::logic_error Throws if there is no value
            const T &Value() const &
            {
                if (mHasValue)
                {
                    return mValue;
                }
                else
                {
                    throw std::logic_error("Result contains no value.");
                }
            }

            /// @brief Get Result possible value
            /// @returns Movable value
            /// @throws std::logic_error Throws if there is no value
            T &&Value() &&
            {
                if (mHasValue)
                {
                    return std::move(mValue);
                }
                else
                {
                    throw std::logic_error("Result contains no value.");
                }
            }

            /// @brief Get Result possible error
            /// @returns Copied error
            /// @throws std::logic_error Throws if there is no error
            const E &Error() const &
            {
                if (mHasValue)
                {
                    throw std::logic_error("Result contains no error.");
                }
                else
                {
                    return mError;
                }
            }

            /// @brief Get Result possible error
            /// @returns Movable error
            /// @throws std::logic_error Throws if there is no error
            E &&Error() &&
            {
                if (mHasValue)
                {
                    throw std::logic_error("Result contains no error.");
                }
                else
                {
                    return std::move(mError);
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
                    return mValue;
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
                    return std::move(mValue);
                }
                else
                {
                    return static_cast<T>(defaultValue);
                }
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) const &
            {
                if (mHasValue)
                {
                    return static_cast<E>(defaultError);
                }
                else
                {
                    return mError;
                }
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) &&
            {
                if (mHasValue)
                {
                    return static_cast<E>(defaultError);
                }
                else
                {
                    return std::move(mError);
                }
            }

            /// @brief Check an error with the instance error
            /// @tparam G Input error type
            /// @returns True if the input error equals the instance error, otherwise false
            template <typename G>
            bool CheckError(G &&error) const
            {
                if (mHasValue)
                {
                    // No error exists for comparison
                    return false;
                }
                else
                {
                    E _error = static_cast<E>(error);
                    return mError = _error;
                }
            }

            /// @brief Get instance possible value or throw an exception
            /// @returns Copied value if exists
            /// @throws std::logic_error Throws if there is no value
            const T &ValueOrThrow() const &noexcept(false)
            {
                return Value();
            }

            /// @brief Get instance possible value or throw an exception
            /// @returns Movable value if exists
            /// @throws std::logic_error Throws if there is no value
            T &&ValueOrThrow() &&noexcept(false)
            {
                /// @todo Check if the correct overload will be called or not
                return Value();
            }

            /// @brief Get the instance value or a callable result
            /// @tparam F Callable type
            /// @param f Callable to be invoked if no value exists
            /// @returns Instance value if exists, otherwise the callable result
            template <typename F>
            T Resolve(F &&f) const
            {
                if (mHasValue)
                {
                    return mValue;
                }
                else
                {
                    f(mError);
                }
            }

            /// @brief Create a new Result by passing the instance value (if exists) to a callable
            /// @tparam F Callable type
            /// @param f Callable that takes the instance value and returns a new Result
            /// @returns Callable result if the value exists, otherwise a new Result with the copied instance error
            template <typename F>
            auto Bind(F &&f) const
            {
                if (mHasValue)
                {
                    return f(mValue);
                }
                else
                {
                    return Result{mError};
                }
            }
        };

        template <typename E>
        class Result<void, E> final
        {
        public:
            using value_type = void;
            using error_type = E;

            Result() noexcept;
            Result(const E &e);
            Result(E &&e);
            Result(const Result &other);
            Result(Result &&other) noexcept(
                std::is_nothrow_move_constructible<E>::value);
            ~Result();
            Result &operator=(Result const &other);
            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_constructible<E>::value &&
                    std::is_nothrow_move_assignable<E>::value);
            void Swap(Result &other) noexcept(
                std::is_nothrow_move_constructible<E>::value &&
                    std::is_nothrow_move_assignable<E>::value);
            explicit operator bool() const noexcept;
            const E &Error() const &;
            E &&Error() &&;

            static Result FromValue();
            static Result FromError(const E &e);
            static Result FromError(E &&e);
        };
    }
}

#endif