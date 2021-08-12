#include <gtest/gtest.h>
#include "../../../src/ara/sm/power_mode.h"
#include "../../../src/ara/com/cg/communication_group_server.h"
#include "../../../src/ara/com/cg/communication_group_client.h"

namespace ara
{
    namespace sm
    {
        class PowerModeTest : public testing::Test
        {
        private:
            const uint32_t cClientID = 0;
            const PowerModeRespMsg cResponse = PowerModeRespMsg::kDone;

        protected:
            ara::com::cg::CommunicationGroupServer<PowerModeMsg, PowerModeRespMsg> Server;
            ara::com::cg::CommunicationGroupClient<PowerModeMsg, PowerModeRespMsg> Client;

            PowerModeTest() : Server(std::bind(&PowerModeTest::ServerHandler, this, std::placeholders::_1, std::placeholders::_2)),
                              Client(std::bind(&PowerModeTest::ClientHandler, this, std::placeholders::_1))
            {
            }

            void ServerHandler(uint32_t clientID, PowerModeRespMsg response)
            {
                EXPECT_EQ(cClientID, clientID);
                EXPECT_EQ(cResponse, response);
            }

            void ClientHandler(PowerModeMsg request)
            {
                Server.Response(cClientID, cResponse);
            }
        };

        TEST_F(PowerModeTest, RequestResponseChain)
        {
            const PowerModeMsg cRequest = PowerModeMsg::kOn;
            EXPECT_NO_THROW(Client.Message(cRequest));
        }
    }
}