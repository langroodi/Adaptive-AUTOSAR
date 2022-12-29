#ifndef ROUTING_ACTIVATION_RESPONSE_TYPE_H
#define ROUTING_ACTIVATION_RESPONSE_TYPE_H

#include <stdint.h>

namespace DoipLib
{
    /// @brief DoIP routing activation response code type
    enum class RoutingActivationResponseType : uint8_t
    {
        InvalidSourceAddress = 0x00,        ///<! Activation denied due to invalid tester logical address
        NoSocketAvailable = 0x01,           ///<! Activation denied due to unavailability of concurrent TCP sockets
        Busy = 0x02,                        ///<! Activation denied due to processing another request
        AlreadyRegisteredTester = 0x03,     ///<! Activation denied due to already tester registeration
        FailedAuthentication = 0x04,        ///<! Activation denied due to authentication missing/failure
        RejectedConfirmation = 0x05,        ///<! Actication denied due to confirmation rejection
        UnsupportedActivationType = 0x06,   ///<! Activation denied due to unsupported requested activation type
        NoSecureSocket = 0x07,              ///<! Activation denied due to TLS connection requirement
        Successful = 0x10,                  ///<! Activation was successful
        Pending = 0x11                      ///<! Activation is pending for confirmation
    };
}

#endif