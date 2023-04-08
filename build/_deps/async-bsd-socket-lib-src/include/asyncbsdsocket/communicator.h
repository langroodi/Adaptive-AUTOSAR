#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <unistd.h>

namespace AsyncBsdSocketLib
{
    /// @brief Communication method (i.e., network sockets and named pipes)
    class Communicator
    {
    protected:
        /// @brief File descriptor
        int FileDescriptor;

        Communicator() noexcept;

    public:
        virtual ~Communicator() noexcept;

        /// @brief File descriptor
        /// @returns A non-negative number if the communicator has been already set up
        int Descriptor() const noexcept;

        /// @brief Connection descriptor for sending and receiving
        /// @returns A non-negative value if there is a valid connection; otherwise -1
        virtual int Connection() const noexcept = 0;

        /// @brief Try to setup the communicator
        /// @returns True if the communicator is successfully set up; otherwise false
        virtual bool TrySetup() noexcept = 0;

        /// @brief Try to close the communicator
        /// @returns True if the communicator closed successfully; otherwise false
        virtual bool TryClose() noexcept;
    };
}

#endif