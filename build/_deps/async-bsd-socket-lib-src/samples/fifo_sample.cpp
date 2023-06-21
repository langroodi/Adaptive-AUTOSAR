#include <iostream>
#include <future>
#include <csignal>
#include "../include/asyncbsdsocket/poller.h"
#include "../include/asyncbsdsocket/fifo_receiver.h"
#include "../include/asyncbsdsocket/fifo_sender.h"

using namespace AsyncBsdSocketLib;

bool running;
Poller *poller;
FifoReceiver *server;
FifoSender *client;

void sigHandler(int signum)
{
    running = false;
}

void onReceive(void)
{
    const std::size_t cBufferSize = 5;

    std::array<uint8_t, cBufferSize> cReceiveBuffer;
    bool _received = server->Receive(cReceiveBuffer) > 0;

    if (_received)
    {
        std::cout << "The server received data from the client.\n";
    }
}

void onSend(void)
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

    std::cout << "Fifo setups were successful.\n";
    return true;
}

void tester(void)
{
    const int cTimeoutMs = 1;
    const std::string cPathname = "/tmp/fifosample";

    poller = new Poller();
    server = new FifoReceiver(cPathname);
    client = new FifoSender(cPathname);

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
