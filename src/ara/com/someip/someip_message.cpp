#include "./someip_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {

            SomeIpMessage::SomeIpMessage(std::uint32_t messageId,
                                         std::uint16_t clientId,
                                         std::uint16_t sessionId,
                                         std::uint8_t protocolVersion,
                                         std::uint8_t interfaceVersion,
                                         SomeIpMessageType messageType,
                                         SomeIpReturnCode returnCode) : mMessageId{messageId},
                                                                        mClientId{clientId},
                                                                        mSessionId{sessionId},
                                                                        mProtocolVersion{protocolVersion},
                                                                        mInterfaceVersion{interfaceVersion},
                                                                        mMessageType{messageType},
                                                                        mReturnCode{returnCode}

            {
            }

            SomeIpMessage::SomeIpMessage(std::uint32_t messageId,
                                         std::uint16_t clientId,
                                         std::uint8_t protocolVersion,
                                         std::uint8_t interfaceVersion,
                                         SomeIpMessageType messageType,
                                         std::uint16_t sessionId) : SomeIpMessage(messageId,
                                                                                  clientId,
                                                                                  sessionId,
                                                                                  protocolVersion,
                                                                                  interfaceVersion,
                                                                                  messageType,
                                                                                  SomeIpReturnCode::eOK)
            {
                if ((messageType != SomeIpMessageType::Request) ||
                    (messageType != SomeIpMessageType::Notification))
                {
                    // E2E is not supported yet.
                    throw std::invalid_argument("Invalid message type.");
                }
            }

            SomeIpMessage::SomeIpMessage(std::uint32_t messageId,
                                         std::uint16_t clientId,
                                         std::uint8_t protocolVersion,
                                         std::uint8_t interfaceVersion,
                                         SomeIpMessageType messageType,
                                         SomeIpReturnCode returnCode,
                                         std::uint16_t sessionId) : SomeIpMessage(messageId,
                                                                                  clientId,
                                                                                  sessionId,
                                                                                  protocolVersion,
                                                                                  interfaceVersion,
                                                                                  messageType,
                                                                                  returnCode)
            {
                if ((messageType != SomeIpMessageType::Request) ||
                    (messageType != SomeIpMessageType::Notification))
                {
                    // E2E is not supported yet.
                    throw std::invalid_argument("Invalid message type.");
                }
                else if ((messageType == SomeIpMessageType::Error) &&
                         (returnCode == SomeIpReturnCode::eOK))
                {
                    // Error message cannot have OK return code.
                    throw std::invalid_argument("Invalid return code.");
                }
            }

            std::uint32_t SomeIpMessage::MessageId() const noexcept
            {
                return mMessageId;
            }

            std::uint32_t SomeIpMessage::Length() noexcept
            {
                const uint32_t cHeaderSize = 8;
                const uint32_t _payloadSize = Payload().size();
                uint32_t _result = cHeaderSize + _payloadSize;

                return _result;
            }

            std::uint16_t SomeIpMessage::ClientId() const noexcept
            {
                return mClientId;
            }

            std::uint16_t SomeIpMessage::SessionId() const noexcept
            {
                return mSessionId;
            }

            void SomeIpMessage::SetSessionId(std::uint16_t sessionId)
            {
                mSessionId = sessionId;
            }

            void SomeIpMessage::IncrementSessionId() noexcept
            {
                mSessionId++;
            }

            std::uint8_t SomeIpMessage::ProtocolVersion() const noexcept
            {
                return mProtocolVersion;
            }

            std::uint8_t SomeIpMessage::InterfaceVersion() const noexcept
            {
                return mInterfaceVersion;
            }

            SomeIpMessageType SomeIpMessage::MessageType() const noexcept
            {
                return mMessageType;
            }

            SomeIpReturnCode SomeIpMessage::ReturnCode() const noexcept
            {
                return mReturnCode;
            }
        }
    }
}