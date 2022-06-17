#include "asyncbsdsocket/communicator.h"

namespace AsyncBsdSocketLib
{
    Communicator::Communicator() noexcept : FileDescriptor{-1}
    {
    }

    int Communicator::Descriptor() const noexcept
    {
        return FileDescriptor;
    }

    bool Communicator::TryClose() noexcept
    {
        // Check the descriptor validity
        if (FileDescriptor == -1)
        {
            return false;
        }

        int _returnCode = close(FileDescriptor);
        bool _result = _returnCode > -1;

        return _result;
    }

    Communicator::~Communicator() noexcept
    {
        TryClose();
    }
}