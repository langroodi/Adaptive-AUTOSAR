#include <gtest/gtest.h>
#include "../../../../../../src/ara/com/someip/sd/fsm/notready_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/initial_wait_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/repetition_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/main_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/stopped_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/service_notseen_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/service_ready_state.h"
#include "../../../../../../src/ara/com/someip/sd/fsm/service_seen_state.h"

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
                        const helper::SdServerState cExpectedState =
                            helper::SdServerState::NotReady;

                        NotReadyState _machineState(nullptr);
                        helper::SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(MachineStateTest, InitialWaitStateContructor)
                    {
                        const helper::SdServerState cPreviousState =
                            helper::SdServerState::NotReady;
                        const helper::SdServerState cExpectedState =
                            helper::SdServerState::InitialWaitPhase;
                        const helper::SdServerState cNextState =
                            helper::SdServerState::RepetitionPhase;
                        const helper::SdServerState cStoppedState =
                            helper::SdServerState::NotReady;
                        const int cInitialDelayMin = 100;
                        const int cInitialDelayMax = 200;
                        const auto cOnTimerExpired = []
                        {
                            // Empty callback
                        };

                        InitialWaitState<helper::SdServerState> _machineState(
                            cExpectedState,
                            cNextState,
                            cStoppedState,
                            cOnTimerExpired,
                            cInitialDelayMin,
                            cInitialDelayMax);

                        helper::SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);

                        EXPECT_NO_THROW(_machineState.Activate(cPreviousState));
                    }

                    TEST(MachineStateTest, RepetitionStateConstructor)
                    {
                        const helper::SdServerState cPreviousState =
                            helper::SdServerState::InitialWaitPhase;
                        const helper::SdServerState cExpectedState =
                            helper::SdServerState::RepetitionPhase;
                        const helper::SdServerState cNextState =
                            helper::SdServerState::MainPhase;
                        const helper::SdServerState cStoppedState =
                            helper::SdServerState::NotReady;
                        const int cRepetitionsMax = 2;
                        const int cRepetitionsBaseDelay = 100;
                        uint32_t _counter = 0;
                        const auto cOnTimerExpired = [&_counter]()
                        {
                            ++_counter;
                        };

                        RepetitionState<helper::SdServerState> _machineState(
                            cExpectedState,
                            cNextState,
                            cStoppedState,
                            cOnTimerExpired,
                            cRepetitionsMax,
                            cRepetitionsBaseDelay);

                        helper::SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);

                        EXPECT_NO_THROW(_machineState.Activate(cPreviousState));
                        // Due to blocking invocation, the counter value should be equal to the maximum repetitions.
                        EXPECT_EQ(_counter, cRepetitionsMax);
                        // Finish the test gracefully
                    }

                    TEST(MachineStateTest, MainStateConstructor)
                    {
                        const helper::SdServerState cPreviousState =
                            helper::SdServerState::RepetitionPhase;
                        const helper::SdServerState cExpectedState =
                            helper::SdServerState::MainPhase;
                        const int cCyclicOfferDelay = 100;
                        const auto cOnTimerExpired = []()
                        {
                            // Empty callback
                        };

                        MainState _machineState(cOnTimerExpired, cCyclicOfferDelay);

                        helper::SdServerState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(MachineStateTest, StoppedStateConstructor)
                    {
                        const helper::SdClientState cExpectedState =
                            helper::SdClientState::Stopped;

                        helper::TtlTimer _ttlTimer;
                        std::condition_variable _conditionVariable;
                        StoppedState _machineState(&_ttlTimer, &_conditionVariable);
                        helper::SdClientState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(MachineStateTest, ServiceNotseenStateConstructor)
                    {
                        const helper::SdClientState cExpectedState =
                            helper::SdClientState::ServiceNotSeen;

                        helper::TtlTimer _ttlTimer;
                        std::condition_variable _conditionVariable;
                        ServiceNotseenState _machineState(&_ttlTimer, &_conditionVariable);
                        helper::SdClientState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(MachineStateTest, ServiceReadyStateConstructor)
                    {
                        const helper::SdClientState cExpectedState =
                            helper::SdClientState::ServiceReady;

                        helper::TtlTimer _ttlTimer;
                        std::condition_variable _conditionVariable;
                        ServiceReadyState _machineState(&_ttlTimer, &_conditionVariable);
                        helper::SdClientState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }

                    TEST(MachineStateTest, ServiceSeenStateConstructor)
                    {
                        const helper::SdClientState cExpectedState =
                            helper::SdClientState::ServiceSeen;

                        helper::TtlTimer _ttlTimer;
                        std::condition_variable _conditionVariable;
                        ServiceSeenState _machineState(&_ttlTimer, &_conditionVariable);
                        helper::SdClientState _actualState = _machineState.GetState();

                        EXPECT_EQ(_actualState, cExpectedState);
                    }
                }
            }
        }
    }
}