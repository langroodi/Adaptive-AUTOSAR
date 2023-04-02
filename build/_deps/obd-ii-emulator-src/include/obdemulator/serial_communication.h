#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include <signal.h>
#include <poll.h>
#include <queue>
#include <future>
#include <asm/termbits.h>
#include "./communication_layer.h"
#include "./packet_buffer.h"

namespace ObdEmulator
{
    /// @brief Serial port communication layer
    class SerialCommunication : public CommunicationLayer
    {
    private:
        static const int cErrorCode{-1};
        static const int cSignal{SIGUSR1};
        static const size_t cSingalFdIndex{0};
        static const size_t cCommunicationFdIndex{1};
        static const size_t cNumberOfFileDescriptors{2};
        static const size_t cReadBufferSize{20};

        const std::string mSerialPort;
        const speed_t mBaudrate;
        const int mTimeout;

        struct pollfd mFileDescriptors[cNumberOfFileDescriptors];
        PacketBuffer mSendBuffer;
        std::promise<bool> mPromise;
        std::future<bool> mFuture;
        std::thread mPollingThread;

        bool trySetupCommunication(int &fileDescriptor) noexcept;
        bool tryReceive();
        bool trySend();
        void tryPoll();

    public:
        /// @brief Constructor
        /// @param serialPort Serial port address
        /// @param baudrate Serial communication baud rate
        /// @param timeout Polling timeout in milliseconds (1 ms by default)
        /// @throws std::runtime_error Throws if the flow control signal creation failed
        SerialCommunication(
            std::string serialPort, speed_t baudrate, int timeout = 1);

        ~SerialCommunication();

        bool TryStart(std::vector<uint8_t> &&configuration) override;

        bool TrySendAsync(std::vector<uint8_t> &&data) override;

        bool TryStop() override;
    };
}

#endif