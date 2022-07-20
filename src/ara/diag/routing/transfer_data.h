#ifndef TRANSFER_DATA_H
#define TRANSFER_DATA_H

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            /// @brief Transfer data direction between diagnostic client and server
            enum class TransferDirection : uint8_t
            {
                None = 0x00,        ///< Not specified transfer data direction
                Download = 0x01,    ///< Transfer data from client to server
                Upload = 0x02       ///< Transfer data from server to client
            };
        }
    }
}

#endif