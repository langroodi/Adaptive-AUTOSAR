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
            Optional<T> mValue;
            Optional<E> mError;

        public:
            /// @brief Result value type alias
            using value_type = T;
            /// @brief Result error type alias
            using error_type = E;

            Result() = delete;

            Result(const T &t) noexcept(
                std::is_nothrow_copy_constructible<T>::value) : mValue{t}
            {
            }

            Result(T &&t) noexcept(
                std::is_nothrow_move_constructible<T>::value) : mValue{std::move(t)}
            {
            }

            explicit Result(const E &e) noexcept(
                std::is_nothrow_copy_constructible<E>::value) : mError{e}
            {
            }

            explicit Result(E &&e) noexcept(
                std::is_nothrow_move_constructible<E>::value) : mError{std::move(e)}
            {
            }

            Result(const Result &other) noexcept(
                std::is_nothrow_copy_assignable<T>::value &&
                    std::is_nothrow_copy_assignable<E>::value)
            {
                // Copy the value if it exists, otherwise copy the error
                if (other.HasValue())
                {
                    mValue = other.mValue;
                }
                else
                {
                    mError = other.mError;
                }
            }

            Result(Result &&other) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                // Move the value if it exists, otherwise move the error
                if (other.HasValue())
                {
                    mValue = std::move(other.mValue);
                }
                else
                {
                    mError = std::move(other.mError);
                }
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
                // Copy the value if it exists and reset the error, otherwise copy the error and reset the value
                if (other.HasValue())
                {
                    mValue = other.mValue;
                    mError.Reset();
                }
                else
                {
                    mError = other.mError;
                    mValue.Reset();
                }

                return *this;
            }

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                // Move the value if it exists and reset the error, otherwise move the error and reset the value
                if (other.HasValue())
                {
                    mValue = std::move(other.mValue);
                    mError.Reset();
                }
                else
                {
                    mError = std::move(other.mError);
                    mValue.Reset();
                }

                return *this;
            }

            /// @brief Construct a new value from the give argument(s) and assign it to the instance value
            /// @param args Argument(s) to construct a new value
            template <typename... Args>
            void EmplaceValue(Args &&...args)
            {
                mValue = T{std::move(args...)};
                mError.Reset();
            }

            /// @brief Construct a new error from the give argument(s) and assign it to the instance error
            /// @param args Argument(s) to construct a new error
            template <typename... Args>
            void EmplaceError(Args &&...args)
            {
                mError = E{std::move(args...)};
                mValue.Reset();
            }

            /// @brief Swap the current instance with another one
            /// @param other Another Result for swapping
            void Swap(Result &other) noexcept(
                std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_assignable<E>::value)
            {
                if (HasValue() && other.HasValue())
                {
                    std::swap(mValue, other.mValue);
                }
                else if (HasValue() && !other.HasValue())
                {
                    mError = std::move(other.mError);
                    other.mValue = std::move(mValue);
                }
                else if (!HasValue() && other.HasValue())
                {
                    mValue = std::move(other.mValue);
                    other.mError = std::move(mError);
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
                return mValue.HasValue();
            }

            /// @returns True if the instance has a value and false if it contains an error
            explicit operator bool() const noexcept
            {
                return HasValue();
            }

            /// @returns Copied value
            /// @throws std::runtime_error Throws if there is no value
            const T &operator*() const &
            {
                return mValue.Value();
            }

            /// @returns Movable value
            /// @throws std::runtime_error Throws if there is no value
            T &&operator*() &&
            {
                return std::move(mValue).Value();
            }

            /// @returns Constant value pointer
            /// @throws std::runtime_error Throws if there is no value
            const T *operator->() const
            {
                return &mValue.Value();
            }

            /// @brief Get instance possible value
            /// @returns Copied value
            /// @throws std::runtime_error Throws if there is no value
            const T &Value() const &
            {
                return mValue.Value();
            }

            /// @brief Get instance possible value
            /// @returns Movable value
            /// @throws std::runtime_error Throws if there is no value
            T &&Value() &&
            {
                return std::move(mValue).Value();
            }

            /// @brief Get instance possible error
            /// @returns Copied error
            /// @throws std::runtime_error Throws if there is no error
            const E &Error() const &
            {
                return mError.Value();
            }

            /// @brief Get instance possible error
            /// @returns Movable error
            /// @throws std::runtime_error Throws if there is no error
            E &&Error() &&
            {
                return std::move(mError).Value();
            }

            /// @brief Get optional instance value
            /// @returns Optional value
            Optional<T> Ok() const &
            {
                return mValue;
            }

            /// @brief Get optional instance value
            /// @returns Optional value
            Optional<T> Ok() &&
            {
                return std::move(mValue);
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() const &
            {
                return mError;
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() &&
            {
                return std::move(mError);
            }

            /// @brief Get the instance value or the default value
            /// @tparam U Default value type
            /// @param defaultValue Value to be return in absence of the instance value
            /// @returns Instance value if exists, otherwise the default value
            template <typename U>
            T ValueOr(U &&defaultValue) const &
            {
                return mValue.ValueOr(defaultValue);
            }

            /// @brief Get the instance value or the default value
            /// @tparam U Default value type
            /// @param defaultValue Value to be return in absence of the instance value
            /// @returns Instance value if exists, otherwise the default value
            template <typename U>
            T ValueOr(U &&defaultValue) &&
            {
                return std::move(mValue).ValueOr(defaultValue);
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) const &
            {
                return mError.ValueOr(defaultError);
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) &&
            {
                return std::move(mError).ValueOr(defaultError);
            }

            /// @brief Check an error with the instance error
            /// @tparam G Input error type
            /// @returns True if the input error equals the instance error, otherwise false
            template <typename G>
            bool CheckError(G &&error) const
            {
                if (HasValue())
                {
                    // No error exists for comparison
                    return false;
                }
                else
                {
                    E _error = static_cast<E>(error);
                    return mError.Value() == _error;
                }
            }

            /// @brief Get instance possible value or throw an exception
            /// @returns Copied value if exists
            /// @throws std::runtime_error Throws if there is no value
            const T &ValueOrThrow() const &noexcept(false)
            {
                return mValue.Value();
            }

            /// @brief Get instance possible value or throw an exception
            /// @returns Movable value if exists
            /// @throws std::runtime_error Throws if there is no value
            T &&ValueOrThrow() &&noexcept(false)
            {
                return std::move(mValue).Value();
            }

            /// @brief Get the instance value or a callable result
            /// @tparam F Callable type
            /// @param f Callable to be invoked if no value exists
            /// @returns Instance value if exists, otherwise the callable result
            template <typename F>
            T Resolve(F &&f) const
            {
                return HasValue() ? Value() : f(Error());
            }

            /// @brief Create a new Result by passing the instance value (if exists) to a callable
            /// @tparam F Callable type
            /// @param f Callable that takes the instance value and returns a new Result
            /// @returns Callable result if the value exists, otherwise a new Result with the copied instance error
            template <typename F>
            auto Bind(F &&f) const -> Result<decltype(f(Value())), E>
            {
                if (HasValue())
                {
                    Result<decltype(f(Value())), E> _result{f(Value())};
                    return _result;
                }
                else
                {
                    Result<decltype(f(Value())), E> _result{Error()};
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
            Result _result{std::move(t)};
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
            Result _result{std::move(e)};
            return _result;
        }

        /// @returns True if both instances contain equal values or equal errors, otherwise false
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

        /// @returns False if both instances contain equal values or equal errors, otherwise true
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
            bool _result = rhs.HasValue() ? false : lhs == rhs.Error();
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
            bool _result = rhs.HasValue() ? true : lhs != rhs.Error();
            return _result;
        }

        /// @brief A wrapper around the callee's possible error
        /// @tparam E Callee's possible error type
        template <typename E>
        class Result<void, E> final
        {
        private:
            Optional<E> mError;

            bool hasError() const noexcept
            {
                return mError.HasValue();
            }

        public:
            /// @brief Void value type alias
            using value_type = void;
            /// @brief Result error type alias
            using error_type = E;

            Result() noexcept = default;

            explicit Result(const E &e) noexcept(
                std::is_nothrow_copy_constructible<E>::value) : mError{e}
            {
            }

            explicit Result(E &&e) noexcept(
                std::is_nothrow_move_constructible<E>::value) : mError{std::move(e)}
            {
            }

            Result(const Result &other) noexcept(
                std::is_nothrow_copy_assignable<E>::value)
            {
                if (other.hasError())
                {
                    mError = other.mError;
                }
            }

            Result(Result &&other) noexcept(
                std::is_nothrow_move_assignable<E>::value)
            {
                if (other.hasError())
                {
                    mError = std::move(other.mError);
                }
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
            static Result FromError(Args &&...args);

            Result &operator=(Result const &other) noexcept(
                std::is_nothrow_copy_assignable<E>::value)
            {
                if (other.hasError())
                {
                    mError = other.mError;
                }
                else
                {
                    mError.Reset();
                }

                return *this;
            }

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_assignable<E>::value)
            {
                if (other.mHasError)
                {
                    mError = std::move(other.mError);
                }
                else
                {
                    mError.Reset();
                }

                return *this;
            }

            /// @brief Construct a new error from the give argument(s) and assign it to the instance error
            /// @param args Argument(s) to construct a new error
            template <typename... Args>
            void EmplaceError(Args &&...args)
            {
                mError = E{args...};
            }

            /// @brief Swap the current instance with another one
            /// @param other Another Result for swapping
            void Swap(Result &other) noexcept(
                std::is_nothrow_move_assignable<E>::value)
            {
                if (hasError() && other.hasError())
                {
                    std::swap(mError, other.mError);
                }
                else if (hasError() && !other.hasError())
                {
                    other.mError = std::move(mError);
                }
                else if (!hasError() && other.hasError())
                {
                    mError = std::move(other.mError);
                }
            }

            /// @brief Indicate whether the instance contains an error or not
            /// @returns True if the instance has no error, otherwise false
            constexpr bool HasValue() const noexcept
            {
                return !hasError();
            }

            /// @returns True if the instance has no error, otherwise false
            constexpr explicit operator bool() const noexcept
            {
                return HasValue();
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
                return mError.Value();
            }

            /// @brief Get instance possible error
            /// @returns Movable error
            /// @throws std::runtime_error Throws if there is no error
            E &&Error() &&
            {
                return std::move(mError).Value();
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() const &
            {
                return mError;
            }

            /// @brief Get optional instance error
            /// @returns Optional error
            Optional<E> Err() &&
            {
                return std::move(mError);
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
                return mError.ValueOr(defaultError);
            }

            /// @brief Get the instance error or the default error
            /// @tparam G Default error type
            /// @param defaultError Error to be return in absence of the instance error
            /// @returns Instance error if exists, otherwise the default error
            template <typename G>
            E ErrorOr(G &&defaultError) &&
            {
                return std::move(mError).ValueOr(defaultError);
            }

            /// @brief Check an error with the instance error
            /// @tparam G Input error type
            /// @returns True if the input error equals the instance error, otherwise false
            template <typename G>
            bool CheckError(G &&error) const
            {
                if (hasError())
                {
                    E _error = static_cast<E>(error);
                    return mError.Value() == _error;
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
                if (hasError())
                {
                    f(Error());
                }
            }

            /// @brief Create a new Result by invoking a callable
            /// @tparam F Callable type
            /// @param f Callable that returns a new Result
            /// @returns Callable result if the instance does not contain an error, otherwise a new Result with the copied instance error
            template <typename F>
            auto Bind(F &&f) const -> Result<decltype(f()), E>
            {
                if (hasError())
                {
                    Result<decltype(f()), E> _result{Error()};
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
            Result _result{std::move(e)};
            return _result;
        }

        template <typename E>
        template <typename... Args>
        Result<void, E> Result<void, E>::FromError(Args &&...args)
        {
            E _error{std::move(args...)};
            Result _result{_error};

            return _result;
        }

        /// @returns True if both instances contain no errors or equal errors, otherwise false
        template <typename E>
        inline bool operator==(const Result<void, E> &lhs, const Result<void, E> &rhs)
        {
            bool _result;

            if (lhs.HasValue() && rhs.HasValue())
            {
                _result = true;
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

        /// @returns False if both instances contain no errors or equal errors, otherwise true
        template <typename E>
        inline bool operator!=(const Result<void, E> &lhs, const Result<void, E> &rhs)
        {
            bool _result;

            if (lhs.HasValue() && rhs.HasValue())
            {
                _result = false;
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
    }
}

#endif