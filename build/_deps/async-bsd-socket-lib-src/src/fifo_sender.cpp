#include <fcntl.h>
#include "../include/asyncbsdsocket/fifo_sender.h"

namespace AsyncBsdSocketLib
{

    FifoSender::FifoSender(std::string pathname) : mPathname{pathname}
    {
    }

    int FifoSender::Connection() const noexcept
    {
        return FileDescriptor;
    }

    bool FifoSender::TrySetup() noexcept
    {
        // rw- rw- rw-
        const mode_t cMode = 0666;
        bool _result = mkfifo(mPathname.c_str(), cMode);

        if (_result)
        {
            FileDescriptor = open(mPathname.c_str(), O_WRONLY | O_NONBLOCK);
            _result = (FileDescriptor >= 0);
        }

        return _result;
    }
}