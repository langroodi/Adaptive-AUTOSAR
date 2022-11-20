#include "./application/platform/execution_management.h"

int main(int argc, char *argv[])
{
    std::map<std::string, std::string> _arguments;
    application::platform::ExecutionManagement _executionManagement;
    _executionManagement.Initialize(_arguments);

    std::getchar();

    int _result{_executionManagement.Terminate()};

    return _result;
}