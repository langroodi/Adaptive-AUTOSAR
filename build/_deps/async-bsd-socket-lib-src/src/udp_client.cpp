#include <netinet/in.h>
#include "asyncbsdsocket/udp_client.h"

namespace AsyncBsdSocketLib
{
    UdpClient::UdpClient(
        std::string ipAddress, uint16_t port) : NetworkSocket(ipAddress, port),
                                                mIsMulticast{false},
                                                mShareAddress{false}
    {
    }

    UdpClient::UdpClient(
        std::string ipAddress,
        uint16_t port,
        std::string nicIpAddress,
        std::string multicastIpAddress,
        bool shareAddress) : NetworkSocket(ipAddress, port),
                             mNicIpAddress{nicIpAddress},
                             mMulticastIpAddress{multicastIpAddress},
                             mIsMulticast{true},
                             mShareAddress{shareAddress}
    {
    }

    int UdpClient::Connection() const noexcept
    {
        return FileDescriptor;
    }

    bool UdpClient::TrySetup() noexcept
    {
        FileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

        bool _result = (FileDescriptor >= 0);

        if (_result && mIsMulticast && mShareAddress)
        {
            int _enable = 1;

            // Enable re-use address flag
            _result =
                (setsockopt(
                     FileDescriptor,
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     (const void *)(&_enable),
                     sizeof(_enable)) > -1);
        }

        if (_result)
        {
            struct sockaddr_in _address;
            _address.sin_addr.s_addr = inet_addr(IpAddress.c_str());
            _address.sin_family = AF_INET;
            _address.sin_port = htons(Port);

            //! \remark Binding the socket to the port
            _result =
                (bind(
                     FileDescriptor,
                     (struct sockaddr *)&_address,
                     sizeof(_address)) == 0);
        }

        // Set multicast socket option if the binding was successful
        if (mIsMulticast && _result)
        {
            struct in_addr _nicAddress;
            _nicAddress.s_addr = inet_addr(mNicIpAddress.c_str());

            // Enable sending multicast traffic through the NIC
            _result =
                (setsockopt(
                     FileDescriptor,
                     IPPROTO_IP, IP_MULTICAST_IF,
                     (const void *)(&_nicAddress),
                     sizeof(_nicAddress)) > -1);

            if (_result)
            {
                struct ip_mreq _multicastGroup;
                _multicastGroup.imr_interface.s_addr =
                    inet_addr(mNicIpAddress.c_str());
                _multicastGroup.imr_multiaddr.s_addr =
                    inet_addr(mMulticastIpAddress.c_str());

                // Subscribe to the multicast group for receiving the multicast traffic
                _result =
                    (setsockopt(FileDescriptor,
                                IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                (const void *)(&_multicastGroup),
                                sizeof(_multicastGroup)) > -1);
            }
        }

        return _result;
    }
}
