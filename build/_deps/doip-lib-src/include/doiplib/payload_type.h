#ifndef PAYLOAD_TYPE_H
#define PAYLOAD_TYPE_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief DoIP message payload type
    enum class PayloadType : uint16_t
    {
        GenericNegativeAcknowledgement = 0x0000,        ///!< Generic DoIP header negative acknowledgement
        VehicleIdRequest = 0x0001,                      ///!< Vehicle identification request
        VehicleIdRequestWithEid = 0x0002,               ///!< Vehicle identification request with specific EID
        VehicleIdRequestWithVin = 0x0003,               ///!< Vehicle identification request with specific VIN
        VehicleAnnoucementIdResponse = 0x0004,          ///!< Vehicle announcement identification response
        RoutingActivationRequest = 0x0005,              ///!< Routing activation phase request
        RoutingActivationResponse = 0x0006,             ///!< Routing activation phase response
        AliveCheckRequest = 0x0007,                     ///!< Alive check heart-beating request
        AliveCheckResponse = 0x0008,                    ///!< Alive check heart-beating response
        DoipEntityStatusRequest = 0x4001,               ///!< DoIP entity (i.e., node or gateway) status request
        DoipEntityStatusResponse = 0x4002,              ///!< DoIP entity status response
        DiagPowerModeInfoRequest = 0x4003,              ///!< Diagnostic power mode information request
        DiagPowerModeInfoResponse = 0x4004,             ///!< Diagnostic power mode information response
        DiagMessage = 0x8001,                           ///!< Diagnostic message (e.g., UDS)
        DiagMessagePositiveAcknowledgement = 0x8002,    ///!< Diagnostic message positive acknowledgement 
        DiagMessageNegativeAcknowledgement = 0x8003     ///!< Diagnostic message negative acknowledgement
    };
}

#endif