#include "./extended_vehicle.h"

namespace application
{
    const std::string ExtendedVehicle::cAppId{"ExtendedVehicle"};

    ExtendedVehicle::ExtendedVehicle() : ara::exec::helper::ModelledProcess(cAppId)
    {
    }

    int ExtendedVehicle::Main(
        const std::atomic_bool *cancellationToken,
        const std::map<std::string, std::string> &arguments)
    {
        ara::log::LogStream _logStream;

        try
        {

            _logStream << "Extended Vehicle AA has been initialized.";
            Log(cLogLevel, _logStream);

            bool _running{true};

            while (!cancellationToken->load() && _running)
            {
                _running = WaitForActivation();
                mPoller.TryPoll();
            }

            _logStream.Flush();
            _logStream << "State management has been terminated.";
            Log(cLogLevel, _logStream);

            return cSuccessfulExitCode;
        }
        catch (const std::runtime_error &ex)
        {
            _logStream.Flush();
            _logStream << ex.what();
            Log(cErrorLevel, _logStream);

            return cUnsuccessfulExitCode;
        }
    }
}