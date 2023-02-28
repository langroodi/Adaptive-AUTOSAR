#ifndef POLLER_H
#define POLLER_H

#include <map>
#include <mutex>
#include <functional>
#include "./tcp_listener.h"

namespace AsyncBsdSocketLib
{
    /// @brief Communicator poller
    class Poller
    {
    private:
        std::mutex mMutex;
        int mFileDescriptor;
        size_t mEventCounter;
        std::map<int, std::function<void()>> mListeners;
        std::map<int, std::function<void()>> mSenders;
        std::map<int, std::function<void()>> mReceivers;

        bool tryModifyAsSenderReceiver(int connectionDescriptor);

    public:
        /// @brief Constructor
        /// @throws std::runtime_exception Throws when the poller creation failed
        Poller();

        ~Poller() noexcept;

        /// @brief Try to add a TCP listener to the poller
        /// @param tcpListener TCP listener socket
        /// @param callback Callback to be fired when the socket is ready to accept a client
        /// @returns True if the socket is added successfully to the poller; otherwise false
        /// @remark The function call is thread-safe.
        bool TryAddListener(TcpListener *tcpListener, std::function<void()> callback);

        /// @brief Try to add a sender to the poller
        /// @param communicator Sender communicator
        /// @param callback Callback to be fired when the communicator is ready to send data
        /// @returns True if the communicator is added successfully to the poller; otherwise false
        /// @remark The function call is thread-safe.
        bool TryAddSender(Communicator *communicator, std::function<void()> callback);

        /// @brief Try to add a receiver to the poller
        /// @param communicator Receiver communicator
        /// @param callback Callback to be fired when the communicator is ready receive data
        /// @returns True if the communicator is added successfully to the poller; otherwise false
        /// @remark The function call is thread-safe.
        bool TryAddReceiver(Communicator *communicator, std::function<void()> callback);

        /// @brief Try to remove the TCP listener from the poller
        /// @param tcpListener Previously added TCP listener socket
        /// @returns True if the socket is removed successfully from the poller; otherwise false
        /// @remark The function call is thread-safe.
        bool TryRemoveListener(TcpListener *tcpListener);

        /// @brief Try to remove the sender from the poller
        /// @param communicator Previously added sender communicator
        /// @returns True if the communicator is removed successfully from the poller; otherwise false
        /// @remark The function call is thread-safe.
        bool TryRemoveSender(Communicator *communicator);

        /// @brief Try to remove the receiver from the poller
        /// @param communicator Previously added receiver communicator
        /// @returns True if the communicator is removed successfully from the poller; otherwise false
        /// @remark The function call is thread-safe.
        bool TryRemoveReceiver(Communicator *communicator);

        /// @brief Try to perform an explicit polling over all the added communicators
        /// @param timeout Polling timeout in milliseconds
        /// @returns True if the polling was successful; otherwise false
        /// @note Zero timeout means immediate return from the polling
        /// @note Timeout '-1' means polling indefinitely until an added communicator becomes available for IO
        bool TryPoll(int timeout = 0) const;
    };
}

#endif