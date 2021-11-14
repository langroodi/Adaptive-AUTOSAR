#include "asyncbsdsocket/network_socket.h"

namespace AsyncBsdSocketLib
{
    NetworkSocket::NetworkSocket(std::string ipAddress, uint16_t port) : mIpAddress{ipAddress},
                                                                         mPort{port},
                                                                         mDescriptor{-1}
    {
    }

    int NetworkSocket::Descriptor() const noexcept
    {
        return mDescriptor;
    }

    bool NetworkSocket::TryClose() noexcept
    {
        // Check the descriptor validity
        if (mDescriptor == -1)
        {
            return false;
        }

        int _returnCode = close(mDescriptor);
        bool _result = _returnCode > -1;

        return _result;
    }

    NetworkSocket::~NetworkSocket() noexcept
    {
        TryClose();
    }
}