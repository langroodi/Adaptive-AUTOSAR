#include "asyncbsdsocket/network_socket.h"

namespace AsyncBsdSocketLib
{
    NetworkSocket::NetworkSocket(std::string ipAddress, uint16_t port) : IpAddress{ipAddress},
                                                                         Port{port}
    {
    }
}