#ifndef SOMEIP_RPC_MESSAGE_H
#define SOMEIP_RPC_MESSAGE_H

#include "../someip_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                /// @brief SOME/IP remote procedure call message
                class SomeIpRpcMessage : public SomeIpMessage
                {
                private:
                    std::vector<uint8_t> mRpcPayload;

                    SomeIpRpcMessage();

                public:
                    /// @brief Constructor for RPC request message by copying the RPC payload
                    /// @param messageId Message ID consisting service and method/event ID
                    /// @param clientId Client ID including ID prefix
                    /// @param sessionId Active session ID
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    /// @param rpcPayload Serialized RPC request object byte vector
                    SomeIpRpcMessage(uint32_t messageId,
                                     uint16_t clientId,
                                     uint16_t sessionId,
                                     uint8_t protocolVersion,
                                     uint8_t interfaceVersion,
                                     const std::vector<uint8_t> &rpcPayload);

                    /// @brief Constructor for RPC request message by moving the RPC payload
                    /// @param messageId Message ID consisting service and method/event ID
                    /// @param clientId Client ID including ID prefix
                    /// @param sessionId Active session ID
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    /// @param rpcPayload Serialized RPC request object byte vector
                    SomeIpRpcMessage(uint32_t messageId,
                                     uint16_t clientId,
                                     uint16_t sessionId,
                                     uint8_t protocolVersion,
                                     uint8_t interfaceVersion,
                                     std::vector<uint8_t> &&rpcPayload);

                    /// @brief Constructor for RPC response or error message
                    /// @param messageId Message ID consisting service and method/event ID
                    /// @param clientId Client ID including ID prefix
                    /// @param sessionId Active session ID
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    /// @param returnCode Message response/error return code
                    /// @param rpcPayload Serialized RPC response/error object byte vector
                    SomeIpRpcMessage(uint32_t messageId,
                                     uint16_t clientId,
                                     uint16_t sessionId,
                                     uint8_t protocolVersion,
                                     uint8_t interfaceVersion,
                                     SomeIpReturnCode returnCode,
                                     const std::vector<uint8_t> &rpcPayload);

                    virtual uint32_t Length() const noexcept override;

                    virtual std::vector<uint8_t> Payload() const override;

                    /// @brief Get the serialized RPC object
                    /// @returns Byte vector constant reference
                    const std::vector<uint8_t> &RpcPayload() const;

                    /// @brief Deserialize a SOME/IP RPC message payload
                    /// @param payload Serialized SOME/IP message payload byte array
                    /// @returns SOME/IP RPC message filled by deserializing the payload
                    /// @throws std::out_of_range Throws when the payload is corrupted
                    static SomeIpRpcMessage Deserialize(const std::vector<uint8_t> &payload);
                };
            }
        }
    }
}

#endif