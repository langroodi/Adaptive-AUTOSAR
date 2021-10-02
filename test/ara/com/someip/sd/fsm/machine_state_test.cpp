#include <gtest/gtest.h>
#include "../../../../../../src/ara/com/someip/sd/fsm/notready_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/initial_wait_state.h"

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
                        const SdServerState cExpectedState =
                            SdServerState::NotReady;

                        NotReadyState _machineState;
                        SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(MachineStateTest, InitialWaitStateContructor)
                    {
                        const SdServerState cPreviousState =
                            SdServerState::NotReady;
                        const SdServerState cExpectedState =
                            SdServerState::InitialWaitPhase;
                        const SdServerState cNextState =
                            SdServerState::RepetitionPhase;
                        const int cInitialDelayMin = 1;
                        const int cInitialDelayMax = 10;

                        InitialWaitState<SdServerState> _machineState(
                            cExpectedState,
                            cNextState,
                            cInitialDelayMin,
                            cInitialDelayMax);

                        SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);

                        EXPECT_NO_THROW(_machineState.Activate(cPreviousState));
                        // The second immediate activation should throw
                        EXPECT_THROW(_machineState.Activate(cPreviousState), std::logic_error);
                    }
                }
            }
        }
    }
}