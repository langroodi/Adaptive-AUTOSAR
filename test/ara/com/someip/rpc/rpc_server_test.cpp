#include <gtest/gtest.h>
#include "../../../../../src/ara/com/someip/rpc/rpc_server.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                class RpcServerTest : public testing::Test, public RpcServer
                {
                private:
                    bool handleWithTrue(
                        const std::vector<uint8_t> &rpcRequestPdu,
                        std::vector<uint8_t> &rpcResponsePdu) const
                    {
                        return true;
                    }

                    bool handleWithFalse(
                        const std::vector<uint8_t> &rpcRequestPdu,
                        std::vector<uint8_t> &rpcResponsePdu) const
                    {
                        return false;
                    }

                protected:
                    const uint16_t cServiceId{1};
                    const uint16_t cTrueMethodId{1};
                    const uint16_t cFalseMethodId{2};
                    const uint16_t cClientId{2};
                    const uint16_t cSessionId{1};
                    const std::vector<uint8_t> cRpcPayload;
                    static const uint8_t cProtocolVersion{1};
                    static const uint8_t cInterfaceVersion{1};

                    RpcServerTest() : RpcServer(cProtocolVersion, cInterfaceVersion)
                    {
                        auto _handleWithTrue{
                            std::bind(
                                &RpcServerTest::handleWithTrue,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2)};
                        SetHandler(cServiceId, cTrueMethodId, _handleWithTrue);

                        auto _handleWithFalse{
                            std::bind(
                                &RpcServerTest::handleWithFalse,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2)};
                        SetHandler(cServiceId, cFalseMethodId, _handleWithFalse);
                    }

                    uint32_t GetMessageId(uint16_t serviceId, uint16_t methodId)
                    {
                        auto _result{static_cast<uint32_t>(serviceId << 16)};
                        _result |= methodId;

                        return _result;
                    }
                };

                const uint8_t RpcServerTest::cProtocolVersion;
                const uint8_t RpcServerTest::cInterfaceVersion;

                TEST_F(RpcServerTest, OkHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eOK};

                    uint32_t _messageId{GetMessageId(cServiceId, cTrueMethodId)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, NotOkHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eNotOk};

                    uint32_t _messageId{GetMessageId(cServiceId, cFalseMethodId)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, UnknownMethodHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eUnknownMethod};
                    const uint16_t cUnknownMethod{3};

                    uint32_t _messageId{GetMessageId(cServiceId, cUnknownMethod)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, UnknownServiceHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eUnknownService};
                    const uint16_t cUnknownService{2};

                    uint32_t _messageId{GetMessageId(cUnknownService, cTrueMethodId)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, WrongMessageTypeHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eWrongMessageType};

                    uint32_t _messageId{GetMessageId(cServiceId, cTrueMethodId)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInterfaceVersion,
                        cExpectedResult,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, WrongProtocolVersionHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eWrongProtocolVersion};
                    const uint16_t cInvalidProtocolVersion{2};

                    uint32_t _messageId{GetMessageId(cServiceId, cTrueMethodId)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cInvalidProtocolVersion,
                        cInterfaceVersion,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, WrongInterfaceVersionHandle)
                {
                    const SomeIpReturnCode cExpectedResult{SomeIpReturnCode::eWrongInterfaceVersion};
                    const uint16_t cInvalidInterfaceVersion{2};

                    uint32_t _messageId{GetMessageId(cServiceId, cTrueMethodId)};
                    SomeIpRpcMessage _request(
                        _messageId,
                        cClientId,
                        cSessionId,
                        cProtocolVersion,
                        cInvalidInterfaceVersion,
                        cRpcPayload);

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(_request.Payload(), _responsePayload)};
                    EXPECT_TRUE(_handled);

                    SomeIpRpcMessage _response{
                        SomeIpRpcMessage::Deserialize(_responsePayload)};
                    SomeIpReturnCode _actualResult{_response.ReturnCode()};
                    EXPECT_EQ(cExpectedResult, _actualResult);
                }

                TEST_F(RpcServerTest, NoHandle)
                {
                    const std::vector<uint8_t> cRequestPayload;

                    std::vector<uint8_t> _responsePayload;
                    bool _handled{
                        TryInvokeHandler(cRequestPayload, _responsePayload)};
                    EXPECT_FALSE(_handled);
                }
            }
        }
    }
}