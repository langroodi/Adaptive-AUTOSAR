#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <string>
#include <stdint.h>
#include <fcntl.h>
#include "./communicator.h"

namespace AsyncBsdSocketLib
{
    /// @brief TCP/IP network socket
    class NetworkSocket : public Communicator
    {
    protected:
        /// @brief Listening/Connecting IP address for server/client
        const std::string IpAddress;

        /// @brief Listening/Connecting port number for server/client
        const uint16_t Port;

        /// @brief Constructor
        /// @param ipAddress Binding IPv4 address
        /// @param port Binding port number
        NetworkSocket(std::string ipAddress, uint16_t port);

    public:
        virtual ~NetworkSocket() noexcept = default;
    };
}

#endif