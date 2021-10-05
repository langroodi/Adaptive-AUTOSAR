#include <gtest/gtest.h>
#include "../../../../../../src/ara/com/someip/sd/fsm/notready_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/initial_wait_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/repetition_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/main_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/stopped_state.h"

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
                        const SdServerState cStoppedState =
                            SdServerState::NotReady;
                        const int cInitialDelayMin = 100;
                        const int cInitialDelayMax = 200;
                        const auto cOnTimerExpired = []
                        {
                            // Empty callback
                        };

                        InitialWaitState<SdServerState> _machineState(
                            cExpectedState,
                            cNextState,
                            cStoppedState,
                            cOnTimerExpired,
                            cInitialDelayMin,
                            cInitialDelayMax);

                        SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);

                        EXPECT_NO_THROW(_machineState.Activate(cPreviousState));
                        // The second immediate activation should throw
                        EXPECT_THROW(_machineState.Activate(cPreviousState), std::logic_error);
                        // Finish the test gracefully
                        _machineState.Join();
                    }

                    TEST(MachineStateTest, RepetitionStateConstructor)
                    {
                        const SdServerState cPreviousState =
                            SdServerState::InitialWaitPhase;
                        const SdServerState cExpectedState =
                            SdServerState::RepetitionPhase;
                        const SdServerState cNextState =
                            SdServerState::MainPhase;
                        const SdServerState cStoppedState =
                            SdServerState::NotReady;
                        const int cRepetitionsMax = 2;
                        const int cRepetitionsBaseDelay = 100;
                        uint32_t _counter = 0;
                        const auto cOnTimerExpired = [&_counter]()
                        {
                            ++_counter;
                        };

                        RepetitionState<SdServerState> _machineState(
                            cExpectedState,
                            cNextState,
                            cStoppedState,
                            cOnTimerExpired,
                            cRepetitionsMax,
                            cRepetitionsBaseDelay);

                        SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);

                        EXPECT_NO_THROW(_machineState.Activate(cPreviousState));
                        // The second immediate activation should throw
                        EXPECT_THROW(_machineState.Activate(cPreviousState), std::logic_error);
                        // Due to async invocation, the counter value should be less than the maximum repetitions.
                        EXPECT_LT(_counter, cRepetitionsMax);
                        // Finish the test gracefully
                        _machineState.Join();
                    }

                    TEST(MachineStateTest, MainStateConstructor)
                    {
                        const SdServerState cPreviousState =
                            SdServerState::RepetitionPhase;
                        const SdServerState cExpectedState =
                            SdServerState::MainPhase;
                        const int cCyclicOfferDelay = 100;
                        const auto cOnTimerExpired = []()
                        {
                            // Empty callback
                        };

                        MainState _machineState(cOnTimerExpired, cCyclicOfferDelay);

                        SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);

                        EXPECT_NO_THROW(_machineState.Activate(cPreviousState));
                        // The second immediate activation should throw
                        EXPECT_THROW(_machineState.Activate(cPreviousState), std::logic_error);
                        // Stop the service, otherwise FSM never get out of the main phase
                        _machineState.ServiceStopped();
                        // Finish the test gracefully
                        _machineState.Join();
                    }

                    TEST(MachineStateTest, StoppedStateConstructor)
                    {
                        const SdClientState cExpectedState =
                            SdClientState::Stopped;

                        StoppedState _machineState;
                        SdClientState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }
                }
            }
        }
    }
}