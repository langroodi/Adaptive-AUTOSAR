#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <array>
#include <stdexcept>
#include "./network_socket.h"

namespace AsyncBsdSocketLib
{
    /// @brief TCP client to connect to a TcpListener
    /// @see TcpListener
    class TcpClient : public NetworkSocket
    {
    private:
        struct sockaddr_in mAddress;
        bool mIsConnected;

    public:
        TcpClient() = delete;

        /// @brief Constructor
        /// @param ipAddress Server IP address
        /// @param port Server port number
        TcpClient(std::string ipAddress, uint16_t port);

        int Connection() const noexcept override;

        /// @brief Inidicates whether the client is connected or not
        /// @returns True if the client is connected; otherwise false
        bool IsConnected() const noexcept;

        bool TrySetup() noexcept override;

        /// @brief Try to connect to the server
        /// @returns True if the client is successfully connected; otherwise false
        bool TryConnect() noexcept;

        /// @brief Send a byte array to the connected client
        /// @tparam N Send buffer size
        /// @param buffer Send buffer byte array
        /// @returns Size of sent bytes (-1 in case of sending failed)
        template <std::size_t N>
        ssize_t Send(const std::array<uint8_t, N> &buffer) const noexcept
        {
            ssize_t _result =
                send(
                    FileDescriptor,
                    buffer.data(),
                    N,
                    MSG_NOSIGNAL);

            return _result;
        }

        /// @brief Receive a byte array from the connected client
        /// @tparam N Receive buffer size
        /// @param buffer Receive buffer byte array
        /// @returns Size of received bytes (-1 in case of receiving failed)
        /// @warning Due to edge-triggered polling, starvation can occur
        template <std::size_t N>
        ssize_t Receive(std::array<uint8_t, N> &buffer) const noexcept
        {
            ssize_t _result = recv(FileDescriptor, buffer.data(), N, 0);

            return _result;
        }
    };
}

#endif