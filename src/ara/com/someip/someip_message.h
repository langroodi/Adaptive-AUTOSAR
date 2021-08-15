#ifndef SOMEIP_MESSAGE_H
#define SOMEIP_MESSAGE_H

#include <stdint.h>
#include <stdexcept>
#include <vector>

namespace ara
{
    namespace com
    {
        namespace someip
        {
            /// @brief SOME/IP communication message type
            enum class SomeIpMessageType : std::uint8_t
            {
                Request = 0x00,             ///!< Request expecting a response
                RequestNoReturn = 0x01,     ///!< Fire and forget request
                Notification = 0x02,        ///!< Notification/event callback
                Response = 0x80,            ///!< Response without any error
                Error = 0x81,               ///!< Response containing error
                TpRequest = 0x20,           ///!< Transfer protocol request
                TpRequestNoReturn = 0x21,   ///!< Transfer protocol fire and forget request
                TpNotification = 0x22,      ///!< Transfer protocol notification
                TpResponse = 0xa0,          ///!< Transfer protocol response
                TpError = 0xa1              ///!< Transfer protocol error
            };

            /// @brief SOME/IP communication message return code
            enum class SomeIpReturnCode : std::uint8_t
            {
                eOK,                    ///!< No error occurred
                eNotOk,                 ///!< Unspecified error occurred
                eUnknownService,        ///!< Service ID is unknown
                eUnknownMethod,         ///!< Method ID is unknown
                eNotReady,              ///!< Service is not running
                eNotReachable,          ///!< Service is not reachable
                eTimeout,               ///!< Timeout occurred
                eWrongProtocolVersion,  ///!< SOME/IP protocol version is not supported
                eWrongInterfaceVersion, ///!< Service interface version is not supported
                eMalformedMessage,      ///!< Deserialization error occurred
                eWrongMessageType,      ///!< Invalid message type
                eE2eRepeated,           ///!< Repeated E2E calculation error
                eE2eWrongSequnece,      ///!< Wrong E2E sequence error
                eE2e,                   ///!< Unspecified E2E error
                eE2eNotAvailable,       ///!< E2E is not supported
                eE2eNoNewData           ///!< No new data E2E calculation present
            };

            /// @brief SOME/IP Abstract Message
            class SomeIpMessage
            {
            private:
                std::uint32_t mMessageId;
                std::uint16_t mClientId;
                std::uint16_t mSessionId;
                std::uint8_t mProtocolVersion;
                std::uint8_t mInterfaceVersion;
                SomeIpMessageType mMessageType;
                SomeIpReturnCode mReturnCode;

                SomeIpMessage(std::uint32_t messageId,
                    std::uint16_t clientId,
                    std::uint16_t sessionId,
                    std::uint8_t protocolVersion,
                    std::uint8_t interfaceVersion,
                    SomeIpMessageType messageType,
                    SomeIpReturnCode returnCode);

            protected:
                /// @brief Constructor for request and notification
                /// @param messageId Message ID consisting service and method/event ID
                /// @param clientId Client ID including ID prefix
                /// @param protocolVersion SOME/IP protocol header version
                /// @param interfaceVersion Service interface version
                /// @param messageType SOME/IP message type (request or notification)
                /// @param sessionID Active/non-active session ID
                SomeIpMessage(std::uint32_t messageId,
                    std::uint16_t clientId,
                    std::uint8_t protocolVersion,
                    std::uint8_t interfaceVersion,
                    SomeIpMessageType messageType,
                    std::uint16_t sessionId = 1);
                
                /// @brief Constructor for response and error
                /// @param messageId Message ID consisting service and method/event ID
                /// @param clientId Client ID including ID prefix
                /// @param protocolVersion SOME/IP protocol header version
                /// @param interfaceVersion Service interface version
                /// @param messageType SOME/IP message type (response or error)
                /// @param returnCode Message response/error return code
                /// @param sessionID Active/non-active session ID
                SomeIpMessage(std::uint32_t messageId,
                    std::uint16_t clientId,
                    std::uint8_t protocolVersion,
                    std::uint8_t interfaceVersion,
                    SomeIpMessageType messageType,
                    SomeIpReturnCode returnCode,
                    std::uint16_t sessionId = 1);
                
            public:
                virtual ~SomeIpMessage() noexcept = default;

                /// @brief Get message ID
                /// @returns Message ID consisting service and method/event ID
                std::uint32_t MessageId() const noexcept;

                /// @brief Get message length
                /// @returns Message length including the payload length
                std::uint32_t Length() noexcept;

                /// @brief Get client ID
                /// @returns Client ID including ID prefix
                std::uint16_t ClientId() const noexcept;

                /// @brief Get session ID
                /// @returns Active/non-active session ID
                std::uint16_t SessionId() const noexcept;

                /// @brief Set a new session ID
                /// @param sessionId New session ID
                virtual void SetSessionId(std::uint16_t sessionId);

                /// @brief Increment the session ID by one
                /// @note In the case of wrapping, session ID will start from 1
                void IncrementSessionId() noexcept;

                /// @brief Get protocol version
                /// @returns SOME/IP protocol header version
                std::uint8_t ProtocolVersion() const noexcept;

                /// @brief Get interface version
                /// @returns Service interface version
                std::uint8_t InterfaceVersion() const noexcept;

                /// @brief Get message type
                /// @returns SOME/IP message type
                SomeIpMessageType MessageType() const noexcept;

                /// @brief Get return code
                /// @returns SOME/IP message return code
                SomeIpReturnCode ReturnCode() const noexcept;

                /// @brief Get message payload
                /// @returns Byte array
                virtual const std::vector<std::uint8_t>& Payload() = 0;
            };
        }
    }
}
#endif