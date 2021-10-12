#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <string>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

namespace AsyncBsdSocketLib
{
    /// @brief TCP/IP network socket
    class NetworkSocket
    {
    protected:
        /// @brief Listening/Connecting IP address for server/client
        const std::string mIpAddress;

        /// @brief Listening/Connecting port number for server/client
        const uint16_t mPort;

        /// @brief Socket descriptor
        int mDescriptor;

        /// @brief Constructor
        /// @param ipAddress Binding IPv4 address
        /// @param port Binding port number
        NetworkSocket(std::string ipAddress, uint16_t port);

    public:
        virtual ~NetworkSocket() noexcept;

        /// @brief Socket descriptor
        /// @returns A non-negative number if the socket has been already set up
        int Descriptor() const noexcept;

        /// @brief Connection descriptor for sending and receiving
        /// @returns A non-negative value if there is a valid connection; otherwise -1
        virtual int Connection() const noexcept = 0;

        /// @brief Try to setup the socket
        /// @returns True if the socket is successfully set up; otherwise false
        virtual bool TrySetup() noexcept = 0;

        /// @brief Try to close the socket
        /// @returns True if the socket closed successfully; otherwise false
        virtual bool TryClose() noexcept;
    };
}

#endif