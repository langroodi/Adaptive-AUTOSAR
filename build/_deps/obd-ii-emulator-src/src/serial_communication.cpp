#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include <sys/ioctl.h>
#include "../include/obdemulator/serial_communication.h"

namespace ObdEmulator
{
    SerialCommunication::SerialCommunication(
        std::string serialPort,
        speed_t baudrate,
        int timeout) : mSerialPort{serialPort},
                       mBaudrate{baudrate},
                       mTimeout{timeout}
    {
        sigset_t _signalSet;

        // Create an empty signal set
        bool _succeed{sigemptyset(&_signalSet) > cErrorCode};

        if (_succeed)
        {
            // Add the first Linux user signal to the set
            _succeed = (sigaddset(&_signalSet, cSignal) > cErrorCode);

            if (_succeed)
            {
                // Block the signals within the set to handle them via polling
                _succeed = (sigprocmask(SIG_BLOCK, &_signalSet, NULL) > cErrorCode);

                if (_succeed)
                {
                    // Create a new file descriptor for the signal set for polling
                    int _signalFd{signalfd(-1, &_signalSet, 0)};
                    _succeed = _signalFd > cErrorCode;

                    if (_succeed)
                    {
                        // Add the created signal file descriptor to the polling array
                        mFileDescriptors[cSingalFdIndex].fd = _signalFd;
                        mFileDescriptors[cSingalFdIndex].events = POLLIN;
                    }
                }
            }
        }

        if (!_succeed)
        {
            throw std::runtime_error("The flow control signal creation transaction failed.");
        }
    }

    bool SerialCommunication::trySetupCommunication(int &fileDescriptor) noexcept
    {
        // Open the serial port for reading and writing while disabling the
        // controlling terminal and enabling the non-blocking transmission
        fileDescriptor = open(mSerialPort.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

        bool _result{fileDescriptor > cErrorCode};

        if (_result)
        {
            // Get the serial communication current options
            struct termios2 _options;
            _result &= (ioctl(fileDescriptor, TCGETS2, &_options) > cErrorCode);

            if (_result)
            {
                // Set 8 data bits
                _options.c_cflag &= ~CSIZE;
                _options.c_cflag |= CS8;
                // Set no parity bit
                _options.c_cflag &= ~PARENB;
                // Set one stop bit
                _options.c_cflag &= ~CSTOPB;

                // Enable receiver and local mode to avoid taking the serial port ownership
                _options.c_cflag |= (CLOCAL | CREAD);

                // Enable raw input by disabling the canonical and accordingly echo modes
                _options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
                // Enable raw output
                _options.c_oflag &= ~OPOST;

                // Disable hardware flow control
                _options.c_cflag &= ~CRTSCTS;
                // Disable software flow control
                _options.c_iflag &= ~(IXON | IXOFF | IXANY);

                // Set custome baudrate
                _options.c_cflag &= ~CBAUD;
                _options.c_cflag |= CBAUDEX;
                _options.c_ispeed = mBaudrate;
                _options.c_ospeed = mBaudrate;

                // Set the serial communication new options
                _result &= (ioctl(fileDescriptor, TCSETS2, &_options) > cErrorCode);
            }
        }

        return _result;
    }

    bool SerialCommunication::tryReceive()
    {
        int _communicationFd{mFileDescriptors[cCommunicationFdIndex].fd};
        std::vector<uint8_t> _readBuffer(cReadBufferSize);

        auto _numberOfReadBytes{
            read(_communicationFd, _readBuffer.data(), cReadBufferSize)};

        bool _result;
        if (_numberOfReadBytes == cErrorCode)
        {
            // In case of the read would block or it was interrupted by a signal,
            // set the result to true in order to keep the polling loop running
            _result =
                (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == EINTR);
        }
        else
        {
            if (Callback)
            {
                std::vector<uint8_t> _response;
                bool _handled{Callback(std::move(_readBuffer), _response)};

                // If the query is handled, queue its response for sending
                if (_handled)
                {
                    mSendBuffer.TryEnqueue(std::move(_response));
                }
            }
            else if (AsyncCallback)
            {
                AsyncCallback(std::move(_readBuffer));
            }

            // Regardless of whether the query was handled or not in this case,
            // set the result to true in order to keep the polling loop running
            _result = true;
        }

        return _result;
    }

    bool SerialCommunication::trySend()
    {
        int _communicationFd{mFileDescriptors[cCommunicationFdIndex].fd};

        // To avoid starvation, try sending as many as the send buffer queue size is
        while (!mSendBuffer.Empty())
        {
            std::vector<uint8_t> _response;
            if (mSendBuffer.TryDequeue(_response))
            {
                auto _numberOfSentBytes{
                    write(_communicationFd, _response.data(), _response.size())};

                if ((_numberOfSentBytes == cErrorCode) &&
                    (errno != EAGAIN) && (errno != EWOULDBLOCK) && (errno != EINTR))
                {
                    // In case of the sending failure was NOT because of it would block nor
                    // it was interrupted by a signal, stop the polling loop immediately
                    return false;
                }
            }

            std::this_thread::yield();
        }

        return true;
    }

    void SerialCommunication::tryPoll()
    {
        bool _result{true};
        bool _running{true};

        while (_running)
        {
            int _polledFileDescriptors{
                poll(mFileDescriptors, cNumberOfFileDescriptors, mTimeout)};

            if (_polledFileDescriptors > 0)
            {
                // Check whether the signal has been raised or not
                if (mFileDescriptors[cSingalFdIndex].revents & POLLIN)
                {
                    int _signalFd{mFileDescriptors[cSingalFdIndex].fd};
                    struct signalfd_siginfo _signalInfo;

                    auto _readSize{read(_signalFd, &_signalInfo, sizeof(_signalInfo))};

                    // The read size should be at least equal to the size of the singal info.
                    _result = _readSize == sizeof(_signalInfo);
                    _running = false;
                }
                else
                {
                    if (mFileDescriptors[cCommunicationFdIndex].revents & POLLIN)
                    {
                        _result &= tryReceive();
                    }

                    if (mFileDescriptors[cCommunicationFdIndex].revents & POLLOUT)
                    {
                        _result &= trySend();
                    }

                    // Break the loop if the receiving nor send failed
                    _running = _result;
                }
            }
            else if ((_polledFileDescriptors == cErrorCode) && (errno != EINTR))
            {
                // Only poll again if the polling was intruppted by a signal,
                // otherwise stop the polling loop and return in the error state
                _result = false;
                _running = false;
            }
        }

        mPromise.set_value_at_thread_exit(_result);
    }

    bool SerialCommunication::TryStart(std::vector<uint8_t> &&configuration)
    {
        // To start the communication the future should NOT have a valid state
        bool _result{!mFuture.valid()};
        if (!_result)
        {
            return false;
        }

        int _communictionFd;
        _result = trySetupCommunication(_communictionFd);
        if (!_result)
        {
            return false;
        }

        // Add the created communication file descriptor to the polling array
        mFileDescriptors[cCommunicationFdIndex].fd = _communictionFd;
        mFileDescriptors[cCommunicationFdIndex].events = POLLIN | POLLOUT;

        // Enqueue the configuration packet before polling start
        _result = mSendBuffer.TryEnqueue(std::move(configuration));
        if (!_result)
        {
            return false;
        }

        mFuture = mPromise.get_future();
        mPollingThread = std::thread(&SerialCommunication::tryPoll, this);

        return true;
    }

    bool SerialCommunication::TrySendAsync(std::vector<uint8_t> &&data)
    {
        return mSendBuffer.TryEnqueue(std::move(data));
    }

    bool SerialCommunication::TryStop()
    {
        // To stop the communication the future SHOULD have a valid state
        bool _result{mFuture.valid()};

        if (_result)
        {
            pthread_t _threadHandle{mPollingThread.native_handle()};
            // Raise the first Linux user signal to stop the polling
            _result = pthread_kill(_threadHandle, cSignal) == 0;

            if (_result)
            {
                // Wait for the future which contains the polling loop to finish gracefully
                _result = mFuture.get();
                // Wait for the polling thread to finish gracefully (if possible)
                if (mPollingThread.joinable())
                {
                    mPollingThread.join();
                }

                // Regardless of the polling loop graceful stopping success, clear the send buffer
                _result &= mSendBuffer.TryClear();

                // Regardless of the polling loop graceful stopping success, close the communication
                int _communicationFd{mFileDescriptors[cCommunicationFdIndex].fd};
                _result &= close(_communicationFd) > cErrorCode;
            }
        }

        return _result;
    }

    SerialCommunication::~SerialCommunication()
    {
        TryStop();

        // Regardless of the communication stopping success, release the first Linux user signal
        int _signalFd{mFileDescriptors[cSingalFdIndex].fd};
        close(_signalFd);
    }
}