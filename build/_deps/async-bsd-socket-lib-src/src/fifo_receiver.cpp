#include <fcntl.h>
#include "../include/asyncbsdsocket/fifo_receiver.h"
#include <string.h>

namespace AsyncBsdSocketLib
{

    FifoReceiver::FifoReceiver(std::string pathname) : mPathname{pathname}
    {
    }

    int FifoReceiver::Connection() const noexcept
    {
        return FileDescriptor;
    }

    bool FifoReceiver::TrySetup() noexcept
    {
        // rw- rw- rw-
        const mode_t cMode = 0666;
        bool _result = mkfifo(mPathname.c_str(), cMode) == 0;

        // Open the FIFO file if it is created successfully or it already exists
        if (_result || (errno == EEXIST))
        {
            FileDescriptor = open(mPathname.c_str(), O_RDONLY | O_NONBLOCK);
            _result = (FileDescriptor >= 0);
        }

        return _result;
    }
}