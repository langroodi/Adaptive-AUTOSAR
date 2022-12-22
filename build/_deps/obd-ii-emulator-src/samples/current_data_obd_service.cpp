#include "./current_data_obd_service.h"

namespace ObdEmulator
{

    const uint8_t CurrentDataObdService::cService;

    CurrentDataObdService::CurrentDataObdService() noexcept : ObdService(cService)
    {
    }

    bool CurrentDataObdService::TryGetResponse(
        const std::vector<uint8_t> &pid, std::vector<uint8_t> &response)
    {
        const size_t cPidIndex{0};
        bool _result{true};
        uint8_t _queriedPid{pid.at(cPidIndex)};

        switch (_queriedPid)
        {
        case 0x00:
            // Most significant 4 bits                0000 0000 0000 0001 1111 1111 1111 1112
            // Least significant 4 bits               1234 5678 9abc def0 1234 5678 9abc def0
            // Supported PIDs within [0x01:0x20] are {0011 1110 0111 1111 1111 0000 0001 0101}.
            response = {0x3e, 0x7f, 0xf0, 0x15};
            break;
        case 0x03:
            // The first fuel system state is closed loop using the oxygen sensor feedback.
            // The second fuel system does not exist.
            response = {0x02, 0x00};
            break;
        case 0x04:
            // The engine load is 20%,
            response = {0x33};
            break;
        case 0x05:
            // The coolant temperature is 70 °C.
            response = {0x6e};
            break;
        case 0x06:
            // Short Term Fuel Trim (STFT) of bank 1 is ~10%.
            response = {0x8c};
            break;
        case 0x07:
            // Long Term Fuel Trim (LTFT) of bank 1 is 0%.
            response = {0x80};
            break;
        case 0x0a:
            // The fuel gauge pressure is 240 kPa.
            response = {0x50};
            break;
        case 0x0b:
            // The intake manifold absolute pressure  is 75 kPa.
            response = {0x4b};
            break;
        case 0x0c:
            // The engine speed is 2048 rpm.
            response = {0x20, 0x00};
            break;
        case 0x0d:
            // The vehicle speed is 50 km/h.
            response = {0x32};
            break;
        case 0x0e:
            // Timing advance before TDC is 9°.
            response = {0x92};
            break;
        case 0x0f:
            // Intake air temperature is 40 °C.
            response = {0x50};
            break;
        case 0x10:
            // Mass air flow rate is 5.2 g/s.
            response = {0x02, 0x08};
            break;
        case 0x11:
            // Throttle position is 20%.
            response = {0x33};
            break;
        case 0x12:
            // Secondary air monitor is off.
            response = {0x04};
            break;
        case 0x13:
            // There is only one oxygen sensor in bank 1.
            response = {0x01};
            break;
        case 0x14:
            // The oxygen sensor voltage is 1 V and it is not used to calculate the STFT.
            response = {0xc8, 0xff};
            break;
        case 0x1c:
            // Only OBD-II is supported.
            response = {0x01};
            break;
        case 0x1e:
            // Power Take Off (PTO) is inactive.
            response = {0x00};
            break;
        case 0x20:
            // Most significant 4 bits                2222 2222 2222 2223 3333 3333 3333 3334
            // Least significant 4 bits               1234 5678 9abc def0 1234 5678 9abc def0
            // Supported PIDs within [0x21:0x40] are {1101 0000 0000 0010 0111 0000 0001 0001}.
            response = {0xd0, 0x02, 0x70, 0x11};
            break;
        case 0x21:
            // Distance traveled with malfunction indicator lamp (MIL) on is 0 km.
            response = {0x00, 0x00};
            break;
        case 0x22:
            // Fuel rail absolute pressure ~515 kPa.
            response = {0x19, 0x78};
            break;
        case 0x24:
            // Oxygen sensor 1 lambda is 1 and its voltage is 1 V.
            response = {0x80, 0x00, 0x20, 0x00};
            break;
        case 0x2f:
            // Fuel tank level is ~50%.
            response = {0x80};
            break;
        case 0x32:
            // EVAP canister pressure is 6.8 kPa.
            response = {0x6a, 0x40};
            break;
        case 0x33:
            // Absolute barometric pressure is 101 kPa.
            response = {0x65};
            break;
        case 0x34:
            // Oxygen sensor 1 lambda is 1 and its current is 100 mA.
            response = {0x80, 0x00, 0xe4, 0x00};
            break;
        case 0x3c:
            // Catalyst temperature of bank 1 read from sesnor 1 is 600 °C.
            response = {0x19, 0x00};
            break;
        case 0x40:
            // Most significant 4 bits                4444 4444 4444 4445 5555 5555 5555 5556
            // Least significant 4 bits               1234 5678 9abc def0 1234 5678 9abc def0
            // Supported PIDs within [0x41:0x60] are {0101 1100 0000 1011 1011 0000 1101 0101}.
            response = {0x5c, 0x0b, 0xb0, 0xd5};
            break;
        case 0x42:
            // ECU voltage is 12 V.
            response = {0x2e, 0xe0};
            break;
        case 0x44:
            // Lambda is 1.
            response = {0x80, 0x00};
            break;
        case 0x45:
            // Relative throttle is 20%.
            response = {0x33};
            break;
        case 0x46:
            // Ambient temperature is 35 °C.
            response = {0x4b};
            break;
        case 0x4d:
            // Time running with MIL on is 0 min.
            response = {0x00, 0x00};
            break;
        case 0x4f:
            // Maximum lambda is 2,
            // maximum oxygen sensor voltage is 8 V,
            // maximum oxygen sesnor current is 128 mA, and
            // maximum intake manifold absolute pressure is 250 kPa.
            response = {0x02, 0x08, 0x80, 0x19};
            break;
        case 0x50:
            // Maximum mass air flow rate is 650 g/s.
            response = {0x41, 0x00, 0x00, 0x00};
            break;
        case 0x51:
            // The fuel type is gasoline.
            response = {0x01};
            break;
        case 0x53:
            // EVAP canister absolute pressure is 107.8 kPa.
            response = {0x54, 0x38};
            break;
        case 0x54:
            // EVAP canister gauge pressure is 6.8 kPa.
            response = {0x1a, 0x90};
            break;
        case 0x59:
            // Fuel rail absolute pressure is 200 MPa.
            response = {0x4e, 0x20};
            break;
        case 0x5a:
            // Relative accelerator pedal position is 20%.
            response = {0x33};
            break;
        case 0x5c:
            // Oil temperature is 170 °C.
            response = {0xd2};
            break;
        case 0x5e:
            // The fuel consumption rate is 7 L/h.
            response = {0x00, 0x8c};
            break;
        case 0x60:
            // Most significant 4 bits                6666 6666 6666 6667 7777 7777 7777 7778
            // Least significant 4 bits               1234 5678 9abc def0 1234 5678 9abc def0
            // Supported PIDs within [0x61:0x80] are {1110 0011 0000 0000 0000 0000 0000 0001}.
            response = {0xe3, 0x00, 0x00, 0x01};
            break;
        case 0x61:
            // Driver demanded torque is 20%.
            response = {0x91};
            break;
        case 0x62:
            // Actual engine torque is 20%.
            response = {0x91};
            break;
        case 0x63:
            // Engine reference torque is 256 N.m.
            response = {0x01, 0x00};
            break;
        case 0x67:
            // Only sensor 1 is supported which measures coolant temperature at 70 °C.
            response = {0x01, 0x6e, 0x00};
            break;
        case 0x68:
            // Only sensor 1 is supported which measures intake air temperature at 40 °C.
            response = {0x01, 0x50, 0x00};
            break;
        case 0x80:
            // Most significant 4 bits                8888 8888 8888 8889 9999 9999 9999 999a
            // Least significant 4 bits               1234 5678 9abc def0 1234 5678 9abc def0
            // Supported PIDs within [0x81:0xa0] are {0000 0000 0000 0000 0000 0000 0000 0001}.
            response = {0x00, 0x00, 0x00, 0x01};
            break;
        case 0xa0:
            // Most significant 4 bits                aaaa aaaa aaaa aaab bbbb bbbb bbbb bbbc
            // Least significant 4 bits               1234 5678 9abc def0 1234 5678 9abc def0
            // Supported PIDs within [0xa1:0xc0] are {0000 0100 0000 0000 0000 0000 0000 0000}.
            response = {0x04, 0x00, 0x00, 0x00};
            break;
        case 0xa6:
            // Odometer value is 2560 km.
            response = {0x00, 0x00, 0x64, 0x00};
            break;
        default:
            _result = false;
        }

        return _result;
    }
}