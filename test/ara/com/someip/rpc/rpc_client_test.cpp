#include <gtest/gtest.h>
#include "../../../../../src/ara/com/someip/rpc/rpc_client.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                class RpcClientTest : public testing::Test, public RpcClient
                {
                private:
                    void handleResponse(const SomeIpMessage &response) const
                    {
                        LastSessionId = response.SessionId();
                    }

                protected:
                    const uint16_t cServiceId{1};
                    const uint16_t cMethodId{1};
                    static const uint8_t cProtocolVersion{1};
                    static const uint8_t cInterfaceVersion{1};
                    mutable uint16_t LastSessionId;

                    RpcClientTest() : RpcClient(cProtocolVersion, cInterfaceVersion),
                                      LastSessionId{0}
                    {
                        auto _handleResponse{
                            std::bind(
                                &RpcClientTest::handleResponse,
                                this,
                                std::placeholders::_1)};
                        SetHandler(cServiceId, cMethodId, _handleResponse);
                    }

                    void Send(const std::vector<uint8_t> &payload) override
                    {
                        // Short circuit the send
                        InvokeHandler(payload);
                    }

                public:
                    using RpcClient::Send;
                };

                const uint8_t RpcClientTest::cProtocolVersion;
                const uint8_t RpcClientTest::cInterfaceVersion;

                TEST_F(RpcClientTest, SendMethod)
                {
                    const uint16_t cClientId{2};
                    const std::vector<uint8_t> cRpcPayload;
                    const uint16_t cExpectedResult{1};

                    Send(cServiceId, cMethodId, cClientId, cRpcPayload);
                    EXPECT_EQ(cExpectedResult, LastSessionId);

                    Send(cServiceId, cMethodId, cClientId, cRpcPayload);
                    EXPECT_NE(cExpectedResult, LastSessionId);
                }
            }
        }
    }
}