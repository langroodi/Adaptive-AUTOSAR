#include <unistd.h>
#include <sys/epoll.h>
#include <stdexcept>
#include "asyncbsdsocket/poller.h"

namespace AsyncBsdSocketLib
{
    Poller::Poller()
    {
        mFileDescriptor = epoll_create1(0);

        if (mFileDescriptor == -1)
        {
            throw std::runtime_error("Cannot create the poller");
        }

        mEventCounter = 0;
    }

    Poller::~Poller() noexcept
    {
        close(mFileDescriptor);
    }

    bool Poller::TryAddListener(
        TcpListener *tcpListener, std::function<void()> callback)
    {
        int _socketDescriptor = tcpListener->Descriptor();
        struct epoll_event _event;
        _event.events = EPOLLIN;
        _event.data.fd = _socketDescriptor;

        bool _result =
            (epoll_ctl(
                 mFileDescriptor,
                 EPOLL_CTL_ADD,
                 _socketDescriptor,
                 &_event) >= 0);

        if (_result)
        {
            mListeners[_socketDescriptor] = callback;
            ++mEventCounter;
        }

        return _result;
    }

    bool Poller::TryAddSender(
        Communicator *communicator, std::function<void()> callback)
    {
        int _connectionDescriptor = communicator->Connection();
        struct epoll_event _event;
        _event.events = EPOLLOUT;
        _event.data.fd = _connectionDescriptor;

        bool _result =
            (epoll_ctl(
                 mFileDescriptor,
                 EPOLL_CTL_ADD,
                 _connectionDescriptor,
                 &_event) >= 0);

        if (_result)
        {
            mSenders[_connectionDescriptor] = callback;
            ++mEventCounter;
        }

        return _result;
    }

    bool Poller::TryAddReceiver(
        Communicator *communicator, std::function<void()> callback)
    {
        int _connectionDescriptor = communicator->Connection();
        struct epoll_event _event;
        _event.events = EPOLLIN | EPOLLET;
        _event.data.fd = _connectionDescriptor;

        bool _result =
            (epoll_ctl(
                 mFileDescriptor,
                 EPOLL_CTL_ADD,
                 _connectionDescriptor,
                 &_event) >= 0);

        if (_result)
        {
            mReceivers[_connectionDescriptor] = callback;
            ++mEventCounter;
        }

        return _result;
    }

    bool Poller::TryRemoveListener(TcpListener *tcpListener)
    {
        // To avoid Linux bug, the event should not be NULL pointer
        int _socketDescriptor = tcpListener->Descriptor();
        struct epoll_event _event;
        _event.events = EPOLLIN;
        _event.data.fd = _socketDescriptor;

        bool _result =
            (epoll_ctl(
                 mFileDescriptor,
                 EPOLL_CTL_DEL,
                 _socketDescriptor,
                 &_event) >= 0);

        if (_result)
        {
            mListeners.erase(_socketDescriptor);
            --mEventCounter;
        }

        return _result;
    }

    bool Poller::TryRemoveSender(Communicator *communicator)
    {
        // To avoid Linux bug, the event should not be NULL pointer
        int _connectionDescriptor = communicator->Connection();
        struct epoll_event _event;
        _event.events = EPOLLOUT;
        _event.data.fd = _connectionDescriptor;

        bool _result =
            (epoll_ctl(
                 mFileDescriptor,
                 EPOLL_CTL_DEL,
                 _connectionDescriptor,
                 &_event) >= 0);

        if (_result)
        {
            mSenders.erase(_connectionDescriptor);
            --mEventCounter;
        }

        return _result;
    }

    bool Poller::TryRemoveReceiver(Communicator *communicator)
    {
        // To avoid Linux bug, the event should not be NULL pointer
        int _connectionDescriptor = communicator->Connection();
        struct epoll_event _event;
        _event.events = EPOLLIN | EPOLLET;
        _event.data.fd = _connectionDescriptor;

        bool _result =
            (epoll_ctl(
                 mFileDescriptor,
                 EPOLL_CTL_DEL,
                 _connectionDescriptor,
                 &_event) >= 0);

        if (_result)
        {
            mReceivers.erase(_connectionDescriptor);
            --mEventCounter;
        }

        return _result;
    }

    bool Poller::TryPoll(int timeout) const
    {
        struct epoll_event _events[mEventCounter];

        int _fdNo = epoll_wait(mFileDescriptor, _events, mEventCounter, timeout);

        if (_fdNo == -1)
        {
            return false;
        }

        for (int i = 0; i < _fdNo; ++i)
        {
            int _fd = _events[i].data.fd;

            auto _iterator = mListeners.find(_fd);
            if (_iterator != mListeners.end())
            {
                // Launching the callback
                (_iterator->second)();
            }

            _iterator = mSenders.find(_fd);
            if (_iterator != mSenders.end())
            {
                // Launching the callback
                (_iterator->second)();
            }

            _iterator = mReceivers.find(_fd);
            if (_iterator != mReceivers.end())
            {
                // Launching the callback
                (_iterator->second)();
            }
        }

        return true;
    }
}