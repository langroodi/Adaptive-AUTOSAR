#ifndef CONVERT_H
#define CONVERT_H

#include <algorithm>
#include <array>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>

namespace DoipLib
{
    /// @brief Helper functions namespace for conversion from/to byte vector
    namespace Convert
    {
        static const std::size_t cByteOffset{8};
        static const uint8_t cByteMask{0xff};

        /// @brief Convert the given unsigned integer to a byte vector
        /// @tparam T Unsigned integer type
        /// @param[in] unsignedInteger Unsigned integer to be converted
        /// @param[out] vector Byte vector as the unsigned integer conversion result
        template <typename T>
        void ToByteVector(T unsignedInteger, std::vector<uint8_t> &vector)
        {
            const auto cCastedByteOffset{static_cast<T>(cByteOffset)};
            const std::size_t cUnsignedIntegerSize{sizeof(T)};

            for (std::size_t i = 0; i < cUnsignedIntegerSize; ++i)
            {
                auto _byte{static_cast<uint8_t>(unsignedInteger & cByteMask)};
                vector.insert(vector.begin(), _byte);
                unsignedInteger = unsignedInteger >> cCastedByteOffset;
            }
        }

        /// @brief Convert the given byte vector to an unsigned integer
        /// @tparam T Unsigned integer type
        /// @param vector Byte vector to be converted
        /// @param[inout] offset Offset within the byte vector
        /// @return Unsigned integer as the byte vector conversion result
        template <typename T>
        T ToUnsignedInteger(const std::vector<uint8_t> &vector, std::size_t &offset)
        {
            const auto cCastedByteOffset{static_cast<T>(cByteOffset)};
            const std::size_t cUnsignedIntegerSize{sizeof(T)};

            auto _offsetUpperBound{static_cast<std::size_t>(offset + cUnsignedIntegerSize)};

            T _result{vector.at(offset)};

            for (++offset; offset < _offsetUpperBound; ++offset)
            {
                _result = _result << cCastedByteOffset;
                _result += vector.at(offset);
            }

            return _result;
        }

        /// @brief Convert the given byte vector to an enum
        /// @tparam T Enum with underlying unsigned integer type
        /// @param vector Byte vector to be converted
        /// @param[inout] offset Offset within the byte vector
        /// @return Enum as the byte vector conversion result
        /// @warning If the underlying enum type is not an unsigned integer, the behaviour is undefined.
        template <typename T>
        T ToEnum(const std::vector<uint8_t> &vector, std::size_t &offset)
        {
            auto _resultInt{
                ToUnsignedInteger<typename std::underlying_type<T>::type>(
                    vector, offset)};
            auto _result{static_cast<T>(_resultInt)};

            return _result;
        }

        /// @brief Convert the given byte vector to a byte array
        /// @tparam N Byte array size
        /// @param vector Byte vector to be converted
        /// @param[inout] offset Offset within the byte vector
        /// @return Byte array as the byte vector conversion result
        template <std::size_t N>
        std::array<uint8_t, N> ToByteArray(const std::vector<uint8_t> &vector, std::size_t &offset)
        {
            auto _beginItr = vector.cbegin() + offset;
            std::array<uint8_t, N> _result;
            std::copy_n(_beginItr, N, _result.begin());
            offset += N;

            return _result;
        }

        /// @brief Convert the given byte vector to a string
        /// @tparam N String size
        /// @param vector Byte vector to be converted
        /// @param[inout] offset Offset within the byte vector
        /// @return String as the byte vector conversion result
        template <std::size_t N>
        std::string ToString(const std::vector<uint8_t> &vector, std::size_t &offset)
        {
            auto _beginItr{vector.cbegin() + offset};
            offset += N;
            auto _endItr{vector.cbegin() + offset};
            std::string _result(_beginItr, _endItr);

            return _result;
        }
    }
}

#endif