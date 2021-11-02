#ifndef POLLER_H
#define POLLER_H

#include <map>
#include <functional>
#include "./network_socket.h"
#include "./tcp_listener.h"

namespace AsyncBsdSocketLib
{
    /// @brief Network socket poller
    class Poller
    {
    private:
        int mDescriptor;
        size_t mEventCounter;
        std::map<int, std::function<void()>> mListeners;
        std::map<int, std::function<void()>> mSenders;
        std::map<int, std::function<void()>> mReceivers;

    public:
        /// @brief Constructor
        /// @throws std::runtime_exception Throws when the poller creation failed
        Poller();

        ~Poller() noexcept;

        /// @brief Try to add a TCP listener to the poller
        /// @param tcpListener TCP listener socket
        /// @param callback Callback to be fired when the socket is ready to accept a client
        /// @returns True if the socket is added successfully to the poller; otherwise false
        bool TryAddListener(TcpListener* tcpListener, std::function<void()> callback);

        /// @brief Try to add a sender to the poller
        /// @param networkSocket Sender network socket
        /// @param callback Callback to be fired when the socket is ready to send data
        /// @returns True if the socket is added successfully to the poller; otherwise false
        bool TryAddSender(NetworkSocket* networkSocket, std::function<void()> callback);

        /// @brief Try to add a receiver to the poller
        /// @param networkSocket Receiver network socket
        /// @param callback Callback to be fired when the socket is ready receive data
        /// @returns True if the socket is added successfully to the poller; otherwise false
        bool TryAddReceiver(NetworkSocket* networkSocket, std::function<void()> callback);

        /// @brief Try to remove the TCP listener from the poller
        /// @param tcpListener Previously added TCP listener socket
        /// @returns True if the socket is removed successfully from the poller; otherwise false
        bool TryRemoveListener(TcpListener* tcpListener);

        /// @brief Try to remove the sender from the poller
        /// @param networkSocket Previously added sender network socket
        /// @returns True if the socket is removed successfully from the poller; otherwise false
        bool TryRemoveSender(NetworkSocket* networkSocket);

        /// @brief Try to remove the receiver from the poller
        /// @param networkSocket Previously added receiver network socket
        /// @returns True if the socket is removed successfully from the poller; otherwise false
        bool TryRemoveReceiver(NetworkSocket* networkSocket);

        /// @brief Try to perform an explicit polling over all the added sockets
        /// @param tiemout Polling timeout in milliseconds
        /// @returns True if the polling was successful; otherwise false
        /// @note Zero timeout means immediate return from the polling
        /// @note Timeout '-1' means polling indefinitely until an added socket becomes available for IO
        bool TryPoll(int timeout = 0) const;
    };
}

#endif