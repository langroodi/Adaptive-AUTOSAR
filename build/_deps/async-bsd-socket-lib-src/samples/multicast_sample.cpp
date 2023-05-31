#include <iostream>
#include <future>
#include <csignal>
#include "../include/asyncbsdsocket/poller.h"
#include "../include/asyncbsdsocket/udp_client.h"

using namespace AsyncBsdSocketLib;

static const std::string cMulticastGroup{"239.0.0.1"};
static const std::string cNicIpAddress{"127.0.0.1"};
static const uint16_t cPort{5555};

bool running;
Poller *poller;
UdpClient *server;
UdpClient *client;

void sigHandler(int signum)
{
    running = false;
}

void onReceive(void)
{
    const std::size_t cBufferSize = 5;

    std::array<uint8_t, cBufferSize> _receiveBuffer;

    std::string _ipAddress;
    uint16_t _port;
    bool _received = server->Receive(_receiveBuffer, _ipAddress, _port) > 0;

    if (_received && _ipAddress == cNicIpAddress && _port == cPort)
    {
        std::cout << "The server received data from the client.\n";
    }
}

void onSend(void)
{
    const std::size_t cBufferSize = 5;
    const std::array<uint8_t, cBufferSize> cSendBuffer =
        {0x48, 0x65, 0x6c, 0x6c, 0x6f};

    bool _sent = client->Send(cSendBuffer, cMulticastGroup, cPort) > 0;

    if (_sent)
    {
        std::cout << "The client sent 'Hello' to the server.\n";
    }
}

bool trySetup(void)
{
    bool _succeed = server->TrySetup();
    if (!_succeed)
    {
        std::cout << "Server setup failed.\n";
        return false;
    }

    _succeed = poller->TryAddReceiver(server, &onReceive);
    if (!_succeed)
    {
        std::cout << "Add server to poller failed.\n";
        return false;
    }

    _succeed = client->TrySetup();

    if (!_succeed)
    {
        std::cout << "Client setup failed.\n";
        return false;
    }

    _succeed = poller->TryAddSender(client, &onSend);
    if (!_succeed)
    {
        std::cout << "Add client to poller failed.\n";
        return false;
    }

    std::cout << "Socket setups were successful.\n";
    return true;
}

void tester(void)
{
    const int cTimeoutMs = 1;
    const std::string cAnyIpAddress{"0.0.0.0"};
    const bool cShared{true};

    poller = new Poller();
    server = new UdpClient(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);
    client = new UdpClient(cAnyIpAddress, cPort, cNicIpAddress, cMulticastGroup, true);

    running = trySetup();

    while (running)
    {
        poller->TryPoll(cTimeoutMs);
        std::this_thread::yield();
    }

    delete client;
    delete server;
    delete poller;
}

int main()
{
    std::cout << "Sample has been started. Press Ctrl+C to quit...\n";
    running = true;
    signal(SIGINT, sigHandler);
    std::async(tester);

    return 0;
}
