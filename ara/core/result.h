#include "error_code.h"
#include <type_traits>

namespace ara
{
    namespace core
    {
        template <typename T, typename E = ErrorCode>
        class Result final
        {
        public:
            using value_type = T;
            using error_type = E;

            Result(const T &t);
            Result(T &&t);
            Result(const E &e);
            Result(E &&e);
            Result(const Result &other);
            Result(Result &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&
                    std::is_nothrow_move_constructible<E>::value);
            ~Result();
            Result &operator=(Result const &other);
            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&
                    std::is_nothrow_move_assignable<T>::value &&
                        std::is_nothrow_move_constructible<E>::value &&
                            std::is_nothrow_move_assignable<E>::value);
            void Swap(Result &other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&
                    std::is_nothrow_move_assignable<T>::value &&
                        std::is_nothrow_move_constructible<E>::value &&
                            std::is_nothrow_move_assignable<E>::value);
            bool HasValue() const noexcept;
            explicit operator bool() const noexcept;
            const T &Value() const &;
            T &&Value() &&;
            const E &Error() const &;
            E &&Error() &&;

            static FromValue(const T &t);
            static FromValue(T &&t);
            static FromError(const E &e);
            static FromError(E &&e);
        };
    }
}