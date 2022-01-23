#include <netinet/in.h>
#include <arpa/inet.h>
#include "asyncbsdsocket/tcp_client.h"

namespace AsyncBsdSocketLib
{
    TcpClient::TcpClient(
        std::string ipAddress, uint16_t port) : NetworkSocket(ipAddress, port),
                                                mIsConnected{false}
    {
    }

    int TcpClient::Connection() const noexcept
    {
        return FileDescriptor;
    }

    bool TcpClient::IsConnected() const noexcept
    {
        return mIsConnected;
    }

    bool TcpClient::TrySetup() noexcept
    {
        FileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

        bool _result = (FileDescriptor >= 0);

        if (_result)
        {
            mAddress.sin_addr.s_addr = inet_addr(IpAddress.c_str());
            mAddress.sin_family = AF_INET;
            mAddress.sin_port = htons(Port);
        }

        return _result;
    }

    bool TcpClient::TryConnect() noexcept
    {
        mIsConnected = (connect(
                            FileDescriptor,
                            (struct sockaddr *)&mAddress,
                            sizeof(mAddress)) == 0);

        return mIsConnected;
    }
}