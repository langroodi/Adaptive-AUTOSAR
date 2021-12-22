#ifndef RESULT_H
#define RESULT_H

#include "./error_code.h"
#include "./optional.h"

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

            static void copy(const Result *source, Result *destination) noexcept(
                std::is_nothrow_copy_assignable<T>::value &&
                    std::is_nothrow_copy_assignable<E>::value)
            {
                // Copy the value if exists, otherwise copy the error
                if (source->mHasValue)
                {
                    destination->mValue = source->mValue;
                }
                else
                {
                    destination->mError = source->mError;
                }

                destination->mHasValue = source->mHasValue;
            }

            static void move(Result *source, Result *destination) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                // Move the value if exists, otherwise move the error
                if (source->mHasValue)
                {
                    destination->mValue = std::move(source->mValue);
                }
                else
                {
                    destination->mError = std::move(source->mError);
                }

                destination->mHasValue = source->mHasValue;
            }

        public:
            /// @brief Result value type alias
            using value_type = T;
            /// @brief Result error type alias
            using error_type = E;

            Result() = delete;

            Result(const T &t) noexcept(
                std::is_nothrow_copy_constructible<T>::value) : mValue{t},
                                                                mHasValue{true}
            {
            }

            Result(T &&t) noexcept(
                std::is_nothrow_move_constructible<T>::value) : mValue{t},
                                                                mHasValue{true}
            {
            }

            explicit Result(const E &e) noexcept(
                std::is_nothrow_copy_constructible<E>::value) : mError{e},
                                                                // Result cannot contain both value and error at the same time.
                                                                mHasValue{false}
            {
            }

            explicit Result(E &&e) noexcept(
                std::is_nothrow_move_constructible<E>::value) : mError{e},
                                                                // Result cannot contain both value and error at the same time.
                                                                mHasValue{false}
            {
            }

            Result(const Result &other) noexcept(
                std::is_nothrow_copy_assignable<T>::value &&
                    std::is_nothrow_copy_assignable<E>::value)
            {
                copy(&other, this);
            }

            Result(Result &&other) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                move(&other, this);
            }

            ~Result() noexcept = default;

            /// @brief Result factory by copying its value
            /// @param t Result value
            /// @returns Created Result from the value
            static Result FromValue(const T &t) noexcept(
                std::is_nothrow_copy_constructible<T>::value);

            /// @brief Result factory by moving its value
            /// @param t Result value
            /// @returns Created Result from the value
            static Result FromValue(T &&t) noexcept(
                std::is_nothrow_move_constructible<T>::value);

            /// @brief Result factory by copying its error
            /// @param e Result error
            /// @returns Created Result from the error
            static Result FromError(const E &e) noexcept(
                std::is_nothrow_copy_constructible<E>::value);

            /// @brief Result factory by moving its error
            /// @param e Result error
            /// @returns Created Result from the error
            static Result FromError(E &&e) noexcept(
                std::is_nothrow_move_constructible<E>::value);

            Result &operator=(Result const &other) noexcept(
                std::is_nothrow_copy_assignable<T>::value &&
                    std::is_nothrow_copy_assignable<E>::value)
            {
                copy(&other, this);
                return *this;
            }

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                move(&other, this);
                return *this;
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
                mError = E{args...};
                mHasValue = false;
            }

            /// @brief Swap the current instance with another one
            /// @param other Another Result for swapping
            void Swap(Result &other) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                if (mHasValue && other.mHasValue)
                {
                    std::swap(mValue, other.mValue);
                }
                else if (mHasValue && !other.mHasValue)
                {
                    mError = std::move(other.mError);
                    mHasValue = false;
                    other.mValue = std::move(mValue);
                    other.mHasValue = true;
                }
                else if (!mHasValue && other.mHasValue)
                {
                    mValue = std::move(other.mValue);
                    mHasValue = true;
                    other.mError = std::move(mError);
                    other.mHasValue = false;
                }
                else // Both instances contain error
                {
                    std::swap(mError, other.mError);
                }
            }

            /// @brief Indicate whether the instance has a value or not
            /// @returns True if has a value and false if it contains an error
            bool HasValue() const noexcept
            {
                return mHasValue;
            }

            /// @returns True if the instance has a value and false if it contains an error
            explicit operator bool() const noexcept
            {
                return mHasValue;
            }

            /// @returns Copied value
            /// @throws std::runtime_error Throws if there is no value
            const T &operator*() const &
            {
                if (mHasValue)
                {
                    return mValue;
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @returns Movable value
            /// @throws std::runtime_error Throws if there is no value
            T &&operator*() &&
            {
                if (mHasValue)
                {
                    return std::move(mValue);
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @returns Constant value pointer
            /// @throws std::runtime_error Throws if there is no value
            const T *operator->() const
            {
                if (mHasValue)
                {
                    &mValue;
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @brief Get instance possible value
            /// @returns Copied value
            /// @throws std::runtime_error Throws if there is no value
            const T &Value() const &
            {
                if (mHasValue)
                {
                    return mValue;
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @brief Get instance possible value
            /// @returns Movable value
            /// @throws std::runtime_error Throws if there is no value
            T &&Value() &&
            {
                if (mHasValue)
                {
                    return std::move(mValue);
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @brief Get instance possible error
            /// @returns Copied error
            /// @throws std::runtime_error Throws if there is no error
            const E &Error() const &
            {
                if (mHasValue)
                {
                    throw std::runtime_error("Result contains no error.");
                }
                else
                {
                    return mError;
                }
            }

            /// @brief Get instance possible error
            /// @returns Movable error
            /// @throws std::runtime_error Throws if there is no error
            E &&Error() &&
            {
                if (mHasValue)
                {
                    throw std::runtime_error("Result contains no error.");
                }
                else
                {
                    return std::move(mError);
                }
            }

            /// @brief Get optional instance value
            /// @returns Optional value
            Optional<T> Ok() const &
            {
                Optional<T> _result;

                if (mHasValue)
                {
                    _result = mValue;
                }

                return _result;
            }

            /// @brief Get optional instance value
            /// @returns Optional value
            Optional<T> Ok() &&
            {
                Optional<T> _result;

                if (mHasValue)
                {
                    _result = std::move(mValue);
                }

                return _result;
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() const &
            {
                Optional<E> _result;

                if (!mHasValue)
                {
                    _result = mError;
                }

                return _result;
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() &&
            {
                Optional<E> _result;

                if (!mHasValue)
                {
                    _result = std::move(mError);
                }

                return _result;
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
                    return mError == _error;
                }
            }

            /// @brief Get instance possible value or throw an exception
            /// @returns Copied value if exists
            /// @throws std::runtime_error Throws if there is no value
            const T &ValueOrThrow() const &noexcept(false)
            {
                if (mHasValue)
                {
                    return mValue;
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @brief Get instance possible value or throw an exception
            /// @returns Movable value if exists
            /// @throws std::runtime_error Throws if there is no value
            T &&ValueOrThrow() &&noexcept(false)
            {
                if (mHasValue)
                {
                    return std::move(mValue);
                }
                else
                {
                    throw std::runtime_error("Result contains no value.");
                }
            }

            /// @brief Get the instance value or a callable result
            /// @tparam F Callable type
            /// @param f Callable to be invoked if no value exists
            /// @returns Instance value if exists, otherwise the callable result
            template <typename F>
            T Resolve(F &&f) const
            {
                return mHasValue ? mValue : f(mError);
            }

            /// @brief Create a new Result by passing the instance value (if exists) to a callable
            /// @tparam F Callable type
            /// @param f Callable that takes the instance value and returns a new Result
            /// @returns Callable result if the value exists, otherwise a new Result with the copied instance error
            template <typename F>
            auto Bind(F &&f) const -> Result<decltype(f(Value())), E>
            {
                if (mHasValue)
                {
                    Result<decltype(f(Value())), E> _result{f(mValue)};
                    return _result;
                }
                else
                {
                    Result<decltype(f(Value())), E> _result{mError};
                    return _result;
                }
            }
        };

        template <typename T, typename E>
        Result<T, E> Result<T, E>::FromValue(const T &t) noexcept(
            std::is_nothrow_copy_constructible<T>::value)
        {
            Result _result{t};
            return _result;
        }

        template <typename T, typename E>
        Result<T, E> Result<T, E>::FromValue(T &&t) noexcept(
            std::is_nothrow_move_constructible<T>::value)
        {
            Result _result{t};
            return _result;
        }

        template <typename T, typename E>
        Result<T, E> Result<T, E>::FromError(const E &e) noexcept(
            std::is_nothrow_copy_constructible<E>::value)
        {
            Result _result{e};
            return _result;
        }

        template <typename T, typename E>
        Result<T, E> Result<T, E>::FromError(E &&e) noexcept(
            std::is_nothrow_move_constructible<E>::value)
        {
            Result _result{e};
            return _result;
        }

        /// @returns True if both instances contain equal values or equal erros, otherwise false
        template <typename T, typename E>
        inline bool operator==(const Result<T, E> &lhs, const Result<T, E> &rhs)
        {
            bool _result;

            if (lhs.HasValue() && rhs.HasValue())
            {
                _result = lhs.Value() == rhs.Value();
            }
            else if (!lhs.HasValue() && !rhs.HasValue())
            {
                _result = lhs.Error() == rhs.Error();
            }
            else
            {
                _result = false;
            }

            return _result;
        }

        /// @returns False if both instances contain equal values or equal erros, otherwise true
        template <typename T, typename E>
        inline bool operator!=(const Result<T, E> &lhs, const Result<T, E> &rhs)
        {
            bool _result;

            if (lhs.HasValue() && rhs.HasValue())
            {
                _result = lhs.Value() != rhs.Value();
            }
            else if (!lhs.HasValue() && !rhs.HasValue())
            {
                _result = lhs.Error() != rhs.Error();
            }
            else
            {
                _result = true;
            }

            return _result;
        }

        template <typename T, typename E>
        inline bool operator==(const Result<T, E> &lhs, const T &rhs)
        {
            bool _result = lhs.HasValue() ? lhs.Value() == rhs : false;
            return _result;
        }

        template <typename T, typename E>
        inline bool operator==(const T &lhs, const Result<T, E> &rhs)
        {
            bool _result = rhs.HasValue() ? lhs == rhs.Value() : false;
            return _result;
        }

        template <typename T, typename E>
        inline bool operator!=(const Result<T, E> &lhs, const T &rhs)
        {
            bool _result = lhs.HasValue() ? lhs.Value() != rhs : true;
            return _result;
        }

        template <typename T, typename E>
        inline bool operator!=(const T &lhs, const Result<T, E> &rhs)
        {
            bool _result = rhs.HasValue() ? lhs != rhs.Value() : true;
            return _result;
        }

        template <typename T, typename E>
        inline bool operator==(const Result<T, E> &lhs, const E &rhs)
        {
            bool _result = lhs.HasValue() ? false : lhs.Error() == rhs;
            return _result;
        }

        template <typename T, typename E>
        inline bool operator==(const E &lhs, const Result<T, E> &rhs)
        {
            bool _result = rhs.HasValue() ? false : lhs == rhs.Err();
            return _result;
        }

        template <typename T, typename E>
        inline bool operator!=(const Result<T, E> &lhs, const E &rhs)
        {
            bool _result = lhs.HasValue() ? true : lhs.Error() != rhs;
            return _result;
        }

        template <typename T, typename E>
        inline bool operator!=(const E &lhs, const Result<T, E> &rhs)
        {
            bool _result = rhs.HasValue() ? true : lhs != rhs.Err();
            return _result;
        }

        /// @brief A wrapper around the callee's possible error
        /// @tparam E Callee's possible error type
        template <typename E>
        class Result<void, E> final
        {
        private:
            E mError;
            bool mHasError;

        public:
            /// @brief Void value type alias
            using value_type = void;
            /// @brief Result error type alias
            using error_type = E;

            Result() noexcept : mHasError{false}
            {
            }

            explicit Result(const E &e) noexcept(
                std::is_nothrow_copy_constructible<E>::value) : mError{e},
                                                                mHasError{true}
            {
            }

            explicit Result(E &&e) noexcept(
                std::is_nothrow_move_constructible<E>::value) : mError{e},
                                                                mHasError{true}
            {
            }

            Result(const Result &other) noexcept(
                std::is_nothrow_copy_assignable<E>::value)
            {
                if (other.mHasError)
                {
                    mError = other.mError;
                }

                mHasError = other.mHasError;
            }

            Result(Result &&other) noexcept(
                std::is_nothrow_move_assignable<E>::value)
            {
                if (other.mHasError)
                {
                    mError = std::move(other.mError);
                }

                mHasError = other.mHasError;
            }

            ~Result() noexcept = default;

            /// @brief Void Result factory
            /// @returns Default void Result
            static Result FromValue() noexcept;

            /// @brief Result factory by copying its error
            /// @param e Result error
            /// @returns Created Result from the error
            static Result FromError(const E &e) noexcept(
                std::is_nothrow_copy_constructible<E>::value);

            /// @brief Result factory by moving its error
            /// @param e Result error
            /// @returns Created Result from the error
            static Result FromError(E &&e) noexcept(
                std::is_nothrow_move_constructible<E>::value);

            /// @brief Construct a new error from the give argument(s) and assign it to the instance error
            /// @param args Argument(s) to construct a new error
            template <typename... Args>
            static Result FromError(Args &&...args)
            {
                E _error{args...};
                Result<void, E> _result{_error};
                return _result;
            }

            Result &operator=(Result const &other) noexcept(
                std::is_nothrow_copy_assignable<E>::value)
            {
                if (other.mHasError)
                {
                    mError = other.mError;
                }

                mHasError = other.mHasError;

                return *this;
            }

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_assignable<E>::value)
            {
                if (other.mHasError)
                {
                    mError = std::move(other.mError);
                }

                mHasError = other.mHasError;

                return *this;
            }

            /// @brief Construct a new error from the give argument(s) and assign it to the instance error
            /// @param args Argument(s) to construct a new error
            template <typename... Args>
            void EmplaceError(Args &&...args)
            {
                mError = E{args...};
                mHasError = true;
            }

            /// @brief Swap the current instance with another one
            /// @param other Another Result for swapping
            void Swap(Result &other) noexcept(
                std::is_nothrow_move_assignable<E>::value)
            {
                if (mHasError && other.mHasError)
                {
                    std::swap(mError, other.mError);
                }
                else if (mHasError && !other.mHasError)
                {
                    other.mError = std::move(mError);
                    mHasError = false;
                    other.mHasError = true;
                }
                else if (!mHasError && other.mHasError)
                {
                    mError = std::move(other.mError);
                    mHasError = true;
                    other.mHasError = false;
                }
            }

            /// @brief Indicate that the instance contains no value
            /// @returns Always false
            constexpr bool HasValue() const noexcept
            {
                return false;
            }

            /// @returns False due to the fact that it never can contain a value
            constexpr explicit operator bool() const noexcept
            {
                return false;
            }

            constexpr void operator*() const noexcept
            {
                // The function does nothing and it is implemented to help with generic programming.
            }

            constexpr void operator->() const noexcept
            {
                // The function does nothing and it is implemented to help with generic programming.
            }

            /// @brief The function does nothing
            constexpr void Value() const noexcept
            {
                // The function does nothing and it is implemented to help with generic programming.
            }

            /// @brief Get instance possible error
            /// @returns Copied error
            /// @throws std::runtime_error Throws if there is no error
            const E &Error() const &
            {
                if (mHasError)
                {
                    return mError;
                }
                else
                {
                    throw std::runtime_error("Result contains no error.");
                }
            }

            /// @brief Get instance possible error
            /// @returns Movable error
            /// @throws std::runtime_error Throws if there is no error
            E &&Error() &&
            {
                if (mHasError)
                {
                    return std::move(mError);
                }
                else
                {
                    throw std::runtime_error("Result contains no error.");
                }
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() const &
            {
                Optional<E> _result;

                if (mHasError)
                {
                    _result = mError;
                }

                return _result;
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() &&
            {
                Optional<E> _result;

                if (mHasError)
                {
                    _result = std::move(mError);
                }

                return _result;
            }

            /// @brief The function does nothing
            template <typename U>
            void ValueOr(U &&defaultValue) const noexcept
            {
                // The function does nothing and it is implemented to help with generic programming.
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) const &
            {
                if (mHasError)
                {
                    return mError;
                }
                else
                {
                    return static_cast<E>(defaultError);
                }
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) &&
            {
                if (mHasError)
                {
                    return std::move(mError);
                }
                else
                {
                    return static_cast<E>(defaultError);
                }
            }

            /// @brief Check an error with the instance error
            /// @tparam G Input error type
            /// @returns True if the input error equals the instance error, otherwise false
            template <typename G>
            bool CheckError(G &&error) const
            {
                if (mHasError)
                {
                    E _error = static_cast<E>(error);
                    return mError == _error;
                }
                else
                {
                    // No error exists for comparison
                    return false;
                }
            }

            /// @brief Throw an exception
            /// @throws std::runtime_error Throws always due to the fact that the instance does not contain a value
            void ValueOrThrow() const noexcept(false)
            {
                throw std::runtime_error("Result contains no value.");
            }

            /// @brief Invoke a callable
            /// @tparam F Callable type
            /// @param f Callable to be invoked
            template <typename F>
            void Resolve(F &&f) const
            {
                if (mHasError)
                {
                    f(mError);
                }
            }

            /// @brief Create a new Result by invoking a callable
            /// @tparam F Callable type
            /// @param f Callable that returns a new Result
            /// @returns Callable result if the instance does not contain an error, otherwise a new Result with the copied instance error
            template <typename F>
            auto Bind(F &&f) const -> Result<decltype(f()), E>
            {
                if (mHasError)
                {
                    Result<decltype(f()), E> _result{mError};
                    return _result;
                }
                else
                {
                    Result<decltype(f()), E> _result{f()};
                    return _result;
                }
            }
        };

        template <typename E>
        Result<void, E> Result<void, E>::FromValue() noexcept
        {
            Result _result;
            return _result;
        }

        template <typename E>
        Result<void, E> Result<void, E>::FromError(const E &e) noexcept(
            std::is_nothrow_copy_constructible<E>::value)
        {
            Result _result{e};
            return _result;
        }

        template <typename E>
        Result<void, E> Result<void, E>::FromError(E &&e) noexcept(
            std::is_nothrow_move_constructible<E>::value)
        {
            Result _result{e};
            return _result;
        }
    }
}

#endif