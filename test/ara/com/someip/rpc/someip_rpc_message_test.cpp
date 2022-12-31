#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include "../../../../../src/ara/com/someip/rpc/someip_rpc_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                TEST(SomeIpRpcMessageTest, RequestCopyPayloadConstructor)
                {
                    const uint32_t cMessageId{0};
                    const uint16_t cClientId{0};
                    const uint16_t cSessionId{1};
                    const uint8_t cProtocolVersion{1};
                    const uint8_t cInterfaceVersion{1};
                    const std::vector<uint8_t> cRpcPayload({255});
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Request;

                    SomeIpRpcMessage _message(
                        cMessageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    EXPECT_EQ(_message.RpcPayload(), cRpcPayload);
                    EXPECT_EQ(_message.MessageType(), cMessageType);
                }

                TEST(SomeIpRpcMessageTest, RequestMovePayloadConstructor)
                {
                    const uint32_t cMessageId{0};
                    const uint16_t cClientId{0};
                    const uint16_t cSessionId{1};
                    const uint8_t cProtocolVersion{1};
                    const uint8_t cInterfaceVersion{1};
                    const uint8_t cRpcPayloadByte{255};
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Request;

                    SomeIpRpcMessage _message(
                        cMessageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        std::vector<uint8_t>({cRpcPayloadByte}));

                    EXPECT_EQ(_message.RpcPayload().front(), cRpcPayloadByte);
                    EXPECT_EQ(_message.MessageType(), cMessageType);
                }

                TEST(SomeIpRpcMessageTest, ResponseConstructor)
                {
                    const uint32_t cMessageId{0};
                    const uint16_t cClientId{0};
                    const uint16_t cSessionId{1};
                    const uint8_t cProtocolVersion{1};
                    const uint8_t cInterfaceVersion{1};
                    const SomeIpReturnCode cReturnCode{SomeIpReturnCode::eOK};
                    const std::vector<uint8_t> cRpcPayload({255});
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Response;

                    SomeIpRpcMessage _message(
                        cMessageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cReturnCode,
                        cRpcPayload);

                    EXPECT_EQ(_message.RpcPayload(), cRpcPayload);
                    EXPECT_EQ(_message.MessageType(), cMessageType);
                }

                TEST(SomeIpRpcMessageTest, ErrorConstructor)
                {
                    const uint32_t cMessageId{0};
                    const uint16_t cClientId{0};
                    const uint16_t cSessionId{1};
                    const uint8_t cProtocolVersion{1};
                    const uint8_t cInterfaceVersion{1};
                    const SomeIpReturnCode cReturnCode{SomeIpReturnCode::eUnknownService};
                    const std::vector<uint8_t> cRpcPayload;
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Error;

                    SomeIpRpcMessage _message(
                        cMessageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cReturnCode,
                        cRpcPayload);

                    EXPECT_EQ(_message.MessageType(), cMessageType);
                }

                TEST(SomeIpSdMessageTest, LengthProperty)
                {
                    const uint32_t cMessageId{0x00010002};
                    const uint16_t cClientId{0x02};
                    const uint16_t cSessionId{0x01};
                    const uint8_t cProtocolVersion{0x01};
                    const uint8_t cInterfaceVersion{0x01};
                    const std::vector<uint8_t> cRpcPayload({0xff});
                    const uint32_t cExpectedResult{9};

                    SomeIpRpcMessage _message(
                        cMessageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);
                    uint32_t _actualResult{_message.Length()};

                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST(SomeIpSdMessageTest, PayloadProperty)
                {
                    const uint32_t cMessageId{0x00010002};
                    const uint16_t cClientId{0x02};
                    const uint16_t cSessionId{0x01};
                    const uint8_t cProtocolVersion{0x01};
                    const uint8_t cInterfaceVersion{0x01};
                    const std::vector<uint8_t> cRpcPayload({0x0a, 0x0b, 0x0c, 0x0d});

                    SomeIpRpcMessage _message(
                        cMessageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    const size_t cPayloadSize = 20;
                    const std::array<uint8_t, cPayloadSize> cExpectedPayload =
                        // SOME/IP Header:
                        {0x00, 0x01, 0x00, 0x02,
                         0x00, 0x00, 0x00, 0x0c,
                         0x00, 0x02, 0x00, 0x01,
                         0x01, 0x01, 0x00, 0x00,
                         // RPC payload:
                         0x0a, 0x0b, 0x0c, 0x0d};

                    auto _actualPayload = _message.Payload();

                    bool _areEqual =
                        std::equal(
                            _actualPayload.begin(),
                            _actualPayload.end(),
                            cExpectedPayload.begin());

                    EXPECT_TRUE(_areEqual);
                }

                TEST(SomeIpSdMessageTest, NoRpcPduDeserialization)
                {
                    const std::vector<uint8_t> cPayload =
                        // SOME/IP Header:
                        {0x00, 0x01, 0x00, 0x02,
                         0x00, 0x00, 0x00, 0x08,
                         0x00, 0x02, 0x00, 0x01,
                         0x01, 0x01, 0x00, 0x00};

                    SomeIpRpcMessage _message{SomeIpRpcMessage::Deserialize(cPayload)};
                    EXPECT_TRUE(_message.RpcPayload().empty());
                }

                TEST(SomeIpSdMessageTest, DeserializeMethod)
                {
                    const std::vector<uint8_t> cExpectedResult({0x0a, 0x0b, 0x0c, 0x0d});
                    const std::vector<uint8_t> cPayload =
                        // SOME/IP Header:
                        {0x00, 0x01, 0x00, 0x02,
                         0x00, 0x00, 0x00, 0x0c,
                         0x00, 0x02, 0x00, 0x01,
                         0x01, 0x01, 0x00, 0x00,
                         // RPC payload:
                         0x0a, 0x0b, 0x0c, 0x0d};

                    SomeIpRpcMessage _message{SomeIpRpcMessage::Deserialize(cPayload)};
                    auto _actualResult{_message.RpcPayload()};

                    EXPECT_EQ(cExpectedResult, _actualResult);
                }
            }
        }
    }
}