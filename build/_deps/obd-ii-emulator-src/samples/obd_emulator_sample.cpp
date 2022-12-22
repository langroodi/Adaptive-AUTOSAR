#include <iostream>
#include "./current_data_obd_service.h"
#include "../include/obdemulator/can_driver.h"
#include "../include/obdemulator/obd_emulator.h"
#include "../include/obdemulator/serial_communication.h"

int main()
{
    const std::string cSerialPort{"/dev/ttyUSB0"};
    const speed_t cBaudrate{115200};
    const bool cSupportExtended{false};
    const ObdEmulator::CanBusSpeed cSpeed{ObdEmulator::CanBusSpeed::Speed250kbps};

    ObdEmulator::SerialCommunication serialCommunication(cSerialPort, cBaudrate);
    ObdEmulator::CanDriver canDriver(cSpeed, cSupportExtended);
    ObdEmulator::CurrentDataObdService currentDataObdService;
    ObdEmulator::ObdEmulator obdEmulator(
        &serialCommunication, &canDriver, {&currentDataObdService});

    bool _succeed{obdEmulator.TryStart()};
    if (_succeed)
    {
        std::cout << "Sample has been started successfully. Press any key to quit...\n";
    }
    else
    {
        std::cout << "Sample has been failed to start. Press any key to quit...\n";
    }

    // Avoid the console to be closed
    std::getchar();

    if (_succeed)
    {
        _succeed = obdEmulator.TryStop();

        if (_succeed)
        {
            std::cout << "Sample has been stopped successfully.\n";
        }
        else
        {
            std::cout << "Sample has been failed to gracefully stop.\n";
        }
    }

    return 0;
}