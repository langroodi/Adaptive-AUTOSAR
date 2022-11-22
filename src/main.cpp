#include "./application/platform/execution_management.h"

int main(int argc, char *argv[])
{
    const std::string cConfigArgument{"config"};
    const int cConfigArgumentIndex{1};

    std::map<std::string, std::string> _arguments;

    if (argc > cConfigArgumentIndex)
    {
        std::string _configFilepath{argv[cConfigArgumentIndex]};
        _arguments[cConfigArgument] = _configFilepath;
    }

    application::platform::ExecutionManagement _executionManagement;
    _executionManagement.Initialize(_arguments);

    std::getchar();

    int _result{_executionManagement.Terminate()};

    return _result;
}