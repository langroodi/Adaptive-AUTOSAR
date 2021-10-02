#include <gtest/gtest.h>
#include "../../../../../../src/ara/com/someip/sd/fsm/notready_state.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                namespace fsm
                {
                    TEST(MachineStateTest, NotReadyStateContructor)
                    {
                        const SdServerState cExpectedState = SdServerState::NotReady;

                        NotReadyState _machineState;
                        SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }
                }
            }
        }
    }
}