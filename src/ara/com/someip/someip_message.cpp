#include "./someip_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {

            SomeIpMessage::SomeIpMessage(uint32_t messageId,
                                         uint16_t clientId,
                                         uint16_t sessionId,
                                         uint8_t protocolVersion,
                                         uint8_t interfaceVersion,
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

            SomeIpMessage::SomeIpMessage(uint32_t messageId,
                                         uint16_t clientId,
                                         uint8_t protocolVersion,
                                         uint8_t interfaceVersion,
                                         SomeIpMessageType messageType,
                                         uint16_t sessionId) : SomeIpMessage(messageId,
                                                                             clientId,
                                                                             sessionId,
                                                                             protocolVersion,
                                                                             interfaceVersion,
                                                                             messageType,
                                                                             SomeIpReturnCode::eOK)
            {
                if ((messageType != SomeIpMessageType::Request) &&
                    (messageType != SomeIpMessageType::Notification))
                {
                    // E2E is not supported yet.
                    throw std::invalid_argument("Invalid message type.");
                }
            }

            SomeIpMessage::SomeIpMessage(uint32_t messageId,
                                         uint16_t clientId,
                                         uint8_t protocolVersion,
                                         uint8_t interfaceVersion,
                                         SomeIpMessageType messageType,
                                         SomeIpReturnCode returnCode,
                                         uint16_t sessionId) : SomeIpMessage(messageId,
                                                                             clientId,
                                                                             sessionId,
                                                                             protocolVersion,
                                                                             interfaceVersion,
                                                                             messageType,
                                                                             returnCode)
            {
                if ((messageType == SomeIpMessageType::Request) ||
                    (messageType == SomeIpMessageType::Notification))
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

            SomeIpMessage::SomeIpMessage(SomeIpMessage &&other) noexcept : mMessageId{other.mMessageId},
                                                                           mClientId{other.mClientId},
                                                                           mSessionId{other.mSessionId},
                                                                           mProtocolVersion{other.mProtocolVersion},
                                                                           mInterfaceVersion{other.mInterfaceVersion},
                                                                           mMessageType{other.mMessageType},
                                                                           mReturnCode{other.mReturnCode}
            {
            }

            SomeIpMessage &SomeIpMessage::operator=(SomeIpMessage &&other)
            {
                mMessageId = other.mMessageId;
                mClientId = other.mClientId;
                mSessionId = other.mSessionId;
                mProtocolVersion = other.mProtocolVersion;
                mInterfaceVersion = other.mInterfaceVersion;
                mMessageType = other.mMessageType;
                mReturnCode = other.mReturnCode;

                return *this;
            }

            void SomeIpMessage::Deserialize(
                SomeIpMessage *message,
                const std::vector<uint8_t> &payload)
            {
                // Set the offset at the beginning of the payload
                std::size_t _offset = 0;

                message->mMessageId = helper::ExtractInteger(payload, _offset);

                // Apply the message length field offset
                _offset += 4;

                message->mClientId = helper::ExtractShort(payload, _offset);
                message->mSessionId = helper::ExtractShort(payload, _offset);
                message->mProtocolVersion = payload[_offset++];
                message->mInterfaceVersion = payload[_offset++];
                message->mMessageType =
                    static_cast<SomeIpMessageType>(payload[_offset++]);
                message->mReturnCode =
                    static_cast<SomeIpReturnCode>(payload[_offset]);
            }

            uint32_t SomeIpMessage::MessageId() const noexcept
            {
                return mMessageId;
            }

            uint16_t SomeIpMessage::ClientId() const noexcept
            {
                return mClientId;
            }

            uint16_t SomeIpMessage::SessionId() const noexcept
            {
                return mSessionId;
            }

            void SomeIpMessage::SetSessionId(uint16_t sessionId)
            {
                mSessionId = sessionId;
            }

            bool SomeIpMessage::IncrementSessionId() noexcept
            {
                const uint16_t cSessionIdMin = 1;
                constexpr uint16_t cSessionIdMax = std::numeric_limits<uint16_t>::max();

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

            uint8_t SomeIpMessage::ProtocolVersion() const noexcept
            {
                return mProtocolVersion;
            }

            uint8_t SomeIpMessage::InterfaceVersion() const noexcept
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

            std::vector<uint8_t> SomeIpMessage::Payload() const
            {
                std::vector<uint8_t> _result;

                helper::Inject(_result, MessageId());
                helper::Inject(_result, Length());
                helper::Inject(_result, ClientId());
                helper::Inject(_result, SessionId());
                _result.push_back(ProtocolVersion());
                _result.push_back(InterfaceVersion());

                uint8_t _messageType = static_cast<uint8_t>(MessageType());
                _result.push_back(_messageType);

                uint8_t _returnCode = static_cast<uint8_t>(ReturnCode());
                _result.push_back(_returnCode);

                return _result;
            }
        }
    }
}