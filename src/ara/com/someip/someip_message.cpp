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
                                         SomeIpReturnCode returnCode) noexcept : mMessageId{messageId},
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

            bool SomeIpMessage::IncrementSessionId() noexcept
            {
                const uint8_t cSessionIdMin = 1;
                constexpr uint8_t cSessionIdMax = std::numeric_limits<uint8_t>::max();

                if (mSessionId == cSessionIdMax)
                {
                    mSessionId = cSessionIdMin;
                    return true;
                }
                else
                {
                    mSessionId++;
                    return false;
                }
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

            const std::vector<std::uint8_t> &SomeIpMessage::Payload()
            {
                std::vector<std::uint8_t> _result;

                helper::Inject(_result, MessageId());
                helper::Inject(_result, Length());
                helper::Inject(_result, ClientId());
                helper::Inject(_result, SessionId());
                _result.push_back(ProtocolVersion());
                _result.push_back(InterfaceVersion());

                std::uint8_t _messageType = static_cast<std::uint8_t>(MessageType());
                _result.push_back(_messageType);

                std::uint8_t _returnCode = static_cast<std::uint8_t>(ReturnCode());
                _result.push_back(_returnCode);

                return _result;
            }
        }
    }
}