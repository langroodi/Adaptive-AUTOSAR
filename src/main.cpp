#include "./application/helper/argument_configuration.h"
#include "./application/platform/execution_management.h"

bool running;
AsyncBsdSocketLib::Poller poller;
application::platform::ExecutionManagement *executionManagement;

void performPolling()
{
    const std::chrono::milliseconds cSleepDuration{
        ara::exec::DeterministicClient::cCycleDelayMs};

    while (running)
    {
        poller.TryPoll();
        std::this_thread::sleep_for(cSleepDuration);
    }
}

int main(int argc, char *argv[])
{
    application::helper::ArgumentConfiguration _argumentConfiguration(argc, argv);

    bool _successful{_argumentConfiguration.TryAskingVccApiKey()};
    if (!_successful)
    {
        std::cout << "Asking for the VCC API key is faile!";
        return -1;
    }

    std::system("clear");
    _successful = _argumentConfiguration.TryAskingBearToken();
    if (!_successful)
    {
        std::cout << "Asking for the OAuth 2.0 bear key is failed!";
        return -1;
    }

    running = true;
    executionManagement = new application::platform::ExecutionManagement(&poller);
    executionManagement->Initialize(_argumentConfiguration.GetArguments());

    std::future<void> _future{std::async(std::launch::async, performPolling)};

    std::getchar();
    std::system("clear");
    std::getchar();

    int _result{executionManagement->Terminate()};
    running = false;
    _future.get();
    delete executionManagement;

    return _result;
}