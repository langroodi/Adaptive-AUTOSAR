#ifndef REENTRANCY_H
#define REENTRANCY_H

#include <stdint.h>

namespace ara
{
    namespace diag
    {
        /// @brief Reentrancy type of an instance
        enum class ReentrancyType : int8_t
        {
            kFully = 0x00, ///< Fully reentrant instance
            kNot = 0x01    ///< Not reentrant instance
        };

        /// @brief Reentrancy type of a data ID IO
        struct DataIdentifierReentrancyType
        {
            /// @brief Reentrancy of the data ID reading
            ReentrancyType read;
            /// @brief Reentrancy of the data ID writing
            ReentrancyType write;
            /// @brief Reentrancy of the data ID reading and writing
            ReentrancyType readWrite;
        };
    }
}

#endif