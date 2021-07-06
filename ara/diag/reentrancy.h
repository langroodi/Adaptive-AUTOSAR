#ifndef REENTRANCY_H
#define REENTRANCY_H

#include <stdint.h>

namespace ara
{
    namespace diag
    {
        enum class ReentrancyType : std::uint8_t
        {
            kFully = 0x00,
            kNot = 0x01
        };

        struct DataIdentifierReentrancyType
        {
            ReentrancyType read;
            ReentrancyType write;
            ReentrancyType readWrite;
        };
    }
}

#endif