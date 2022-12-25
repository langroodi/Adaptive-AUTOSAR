#include <utility>
#include <gtest/gtest.h>
#include "../../../src/ara/exec/execution_server.h"
#include "./helper/mock_rpc_server.h"

namespace ara
{
    namespace exec
    {
        class ExecutionServerTest : public testing::Test
        {
        private:
            static const uint8_t cProtocolVersion{1};
            static const uint8_t cInterfaceVersion{1};

            const uint32_t cMessageId{0x00010001};
            const uint16_t cClientId{0x0002};

            helper::MockRpcServer mRpcServer;
            uint16_t mSessionId;

        protected:
            ExecutionServer Server;

            ExecutionServerTest() : mRpcServer(cProtocolVersion, cInterfaceVersion),
                                    Server{&mRpcServer},
                                    mSessionId{0}
            {
            }

            com::someip::rpc::SomeIpRpcMessage Send(
                std::vector<uint8_t> &&rpcPayload)
            {
                com::someip::rpc::SomeIpRpcMessage _request(
                    cMessageId,
                    cClientId,
                    ++mSessionId,
                    cProtocolVersion,
                    cInterfaceVersion,
                    std::move(rpcPayload));

                auto _result{mRpcServer.Send(_request)};

                return _result;
            }
        };

        const uint8_t ExecutionServerTest::cProtocolVersion;
        const uint8_t ExecutionServerTest::cInterfaceVersion;

        TEST_F(ExecutionServerTest, ReportExecutionStateMethod)
        {
            const auto cExpectedReturnCode{com::someip::SomeIpReturnCode::eOK};
            const ExecutionState cExpectedState{ExecutionState::kRunning};
            const std::string cApplicationId{"id"};

            auto _response{Send(std::vector<uint8_t>({0, 0, 0, 2, 105, 100, 0}))};

            auto _actualReturnCode{_response.ReturnCode()};
            EXPECT_EQ(cExpectedReturnCode, _actualReturnCode);

            ExecutionState _actualState;
            EXPECT_TRUE(Server.TryGetExecutionState(cApplicationId, _actualState));
            EXPECT_EQ(cExpectedState, _actualState);
        }
    }
}