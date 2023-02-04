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
    const std::string cDefaultConfigFile{"../configuration/execution_manifest.arxml"};
    const std::string cConfigArgument{"config"};
    const int cConfigArgumentIndex{1};

    const std::string cExtendedVehicleConfigFile{"../configuration/extended_vehicle_manifest.arxml"};
    const std::string cEvConfigArgument{"evconfig"};
    const int cEvConfigArgumentIndex{2};

    const std::string cDiagnosticManagerConfigFile{"../configuration/diagnostic_manager_manifest.arxml"};
    const std::string cDmConfigArgument{"dmconfig"};
    const int cDmConfigArgumentIndex{3};

    std::map<std::string, std::string> _arguments;

    if (argc > cDmConfigArgumentIndex)
    {
        std::string _dmConfigFilepath{argv[cDmConfigArgumentIndex]};
        _arguments[cDmConfigArgument] = _dmConfigFilepath;

        std::string _evConfigFilepath{argv[cEvConfigArgumentIndex]};
        _arguments[cEvConfigArgument] = _evConfigFilepath;

        std::string _configFilepath{argv[cConfigArgumentIndex]};
        _arguments[cConfigArgument] = _configFilepath;
    }
    else
    {
        _arguments[cConfigArgument] = cDefaultConfigFile;
        _arguments[cEvConfigArgument] = cExtendedVehicleConfigFile;
        _arguments[cDmConfigArgument] = cDiagnosticManagerConfigFile;
    }

    running = true;
    executionManagement = new application::platform::ExecutionManagement(&poller);
    executionManagement->Initialize(_arguments);

    std::future<void> _future{std::async(std::launch::async, performPolling)};

    std::getchar();

    int _result{executionManagement->Terminate()};
    running = false;
    _future.get();
    delete executionManagement;

    return _result;
}