#include "./stopped_state.h"

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
                    StoppedState::StoppedState(
                        helper::TtlTimer *ttlTimer,
                        std::condition_variable *conditionVariable) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::Stopped),
                                                                               ClientServiceState(ttlTimer),
                                                                               mConditionVariable{conditionVariable}
                    {
                    }

                    helper::SdClientState StoppedState::waitForNextState()
                    {
                        if (Timer->GetRequested() && !Timer->GetOffered())
                        {
                            Timer->WaitForSignal();
                        }

                        if (!Timer->GetRequested())
                        {
                            return helper::SdClientState::ServiceNotSeen;
                        }
                        else
                        {
                            return helper::SdClientState::ServiceReady;
                        }
                    }

                    void StoppedState::Activate(helper::SdClientState previousState)
                    {
                        // Notify the condition variable that the service is not offered yet
                        mConditionVariable->notify_one();
                        helper::SdClientState _nextState = waitForNextState();
                        Transit(_nextState);
                    }

                    void StoppedState::Deactivate(helper::SdClientState nextState)
                    {
                    }
                };
            }
        }
    }
}