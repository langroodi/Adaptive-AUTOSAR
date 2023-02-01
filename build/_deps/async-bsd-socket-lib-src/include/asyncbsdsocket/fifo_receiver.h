#ifndef FIFO_RECEIVER_H
#define FIFO_RECEIVER_H

#include <sys/stat.h>
#include <string>
#include <array>
#include "./communicator.h"

namespace AsyncBsdSocketLib
{
    /// @brief FIFO (named pipe) IPC receiver (reader)
    class FifoReceiver : public Communicator
    {
    private:
        const std::string mPathname;

    public:
        FifoReceiver() = delete;

        /// @brief Constructor
        /// @param pathname FIFO file path
        FifoReceiver(std::string pathname);

        int Connection() const noexcept override;

        bool TrySetup() noexcept override;

        /// @brief Receive a byte array from the FIFO pipe
        /// @tparam N Receive buffer size
        /// @param buffer Receive buffer byte array
        /// @returns Size of received bytes (-1 in case of receiving failed)
        /// @warning Due to edge-triggered polling, starvation can occur
        template <std::size_t N>
        ssize_t Receive(std::array<uint8_t, N> &buffer) const noexcept
        {
            ssize_t _result = read(FileDescriptor, buffer.data(), N);

            return _result;
        }
    };
}

#endif