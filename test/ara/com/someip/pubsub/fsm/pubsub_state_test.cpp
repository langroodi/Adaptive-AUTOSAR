#include <gtest/gtest.h>
#include "../../../../../../src/ara/com/someip/pubsub/fsm/service_down_state.h"
#include "../../../../../../src/ara/com/someip/pubsub/fsm/notsubscribed_state.h"
#include "../../../../../../src/ara/com/someip/pubsub/fsm/subscribed_state.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                namespace fsm
                {
                    TEST(PubSubStateTest, ServiceDownStateConstructor)
                    {
                        const helper::PubSubState cExpectedState =
                            helper::PubSubState::ServiceDown;

                        ServiceDownState _machineState;
                        helper::PubSubState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(PubSubStateTest, NotSubscribedStateConstructor)
                    {
                        const helper::PubSubState cExpectedState =
                            helper::PubSubState::NotSubscribed;

                        NotSubscribedState _machineState;
                        helper::PubSubState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(PubSubStateTest, SubscribedStateConstructor)
                    {
                        const helper::PubSubState cExpectedState =
                            helper::PubSubState::Subscribed;

                        SubscribedState _machineState;
                        helper::PubSubState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }
                }
            }
        }
    }
}