#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief DoIP enity type
    enum class NodeType : uint8_t
    {
        DoipGateway = 0x00, ///!< DoIP gateway between the vehicle network and external testers
        DoipNode = 0x01     ///!< DoIP node within the vehicle network
    };
}

#endif