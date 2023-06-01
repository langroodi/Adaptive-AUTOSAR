#include <iostream>
#include <future>
#include <csignal>
#include "../include/asyncbsdsocket/poller.h"
#include "../include/asyncbsdsocket/tcp_client.h"

using namespace AsyncBsdSocketLib;

bool running;
Poller *poller;
TcpListener *server;
TcpClient *client;

void sigHandler(int signum)
{
    running = false;
}

void onReceive()
{
    const std::size_t cBufferSize = 5;
    
    std::array<uint8_t, cBufferSize> cReceiveBuffer;
    bool _received = server->Receive(cReceiveBuffer) > 0;

    if (_received)
    {
        std::cout << "The server received data from the client.\n";
    }
}

void onSend()
{
    const std::size_t cBufferSize = 5;
    const std::array<uint8_t, cBufferSize> cSendBuffer =
        {0x48, 0x65, 0x6c, 0x6c, 0x6f};

    bool _sent = client->Send(cSendBuffer) > 0;

    if (_sent)
    {
        std::cout << "The client sent 'Hello' to the server.\n";
    }
}

void onAccept()
{
    bool _accepted = server->TryAccept();
    if (_accepted)
    {
        std::cout << "The server accepted the client.\n";

        server->TryMakeConnectionNonblock();
        poller->TryAddReceiver(server, &onReceive);
        poller->TryAddSender(client, &onSend);
    }
    else
    {
        std::cout << "The server declined the client.\n";
        running = false;
    }
}

void tester()
{
    const int cTimeoutMs = 1;
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t cPort = 9900;

    poller = new Poller();
    server = new TcpListener(cIpAddress, cPort);
    client = new TcpClient(cIpAddress, cPort);

    bool _succeed = server->TrySetup();
    _succeed &= client->TrySetup();
    if (_succeed)
    {
        std::cout << "Socket setups were successful.\n";
        client->TryConnect();
        poller->TryAddListener(server, &onAccept);
    }
    else
    {
        std::cout << "Socket setups failed.\n";
        // Skip the polling loop and releasing the sockets and the poller
        running = false;
    }

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
