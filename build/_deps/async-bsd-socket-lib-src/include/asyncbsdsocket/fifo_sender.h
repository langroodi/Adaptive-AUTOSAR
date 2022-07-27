#ifndef FIFO_SENDER_H
#define FIFO_SENDER_H

#include <sys/stat.h>
#include <string>
#include <array>
#include "./communicator.h"

namespace AsyncBsdSocketLib
{
    /// @brief FIFO (named pipe) IPC sender (writer)
    class FifoSender : public Communicator
    {
    private:
        const std::string mPathname;

    public:
        FifoSender() = delete;

        /// @brief Constructor
        /// @param pathname FIFO file path
        FifoSender(std::string pathname);

        int Connection() const noexcept override;

        bool TrySetup() noexcept override;

        /// @brief Send a byte array to the FIFO pipe
        /// @tparam N Send buffer size
        /// @param buffer Send buffer byte array
        /// @returns Size of sent bytes (-1 in case of sending failed)
        template <std::size_t N>
        ssize_t Send(const std::array<uint8_t, N> &buffer) const noexcept
        {
            ssize_t _result =
                write(
                    FileDescriptor,
                    buffer.data(),
                    N);

            return _result;
        }
    };
}

#endif