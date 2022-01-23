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
                                                                               mConditionVariable{conditionVariable},
                                                                               mNextState{helper::SdClientState::Stopped}
                    {
                    }

                    void StoppedState::Activate(helper::SdClientState previousState)
                    {
                        // Notify the condition variable that the service is not offered yet
                        mConditionVariable->notify_one();

                        if (mNextState == GetState())
                        {
                            Timer->WaitForSignal();
                        }

                        Transit(mNextState);
                    }

                    void StoppedState::ServiceNotRequested()
                    {
                        // Only set the next state if it has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            mNextState = helper::SdClientState::ServiceNotSeen;
                        }
                    }

                    void StoppedState::ServiceOffered(uint32_t ttl) noexcept
                    {
                        // Only set the next state if it has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            mNextState = helper::SdClientState::ServiceReady;
                            Timer->Set(ttl);
                        }
                    }

                    void StoppedState::Deactivate(helper::SdClientState nextState)
                    {
                        // Reset the next state variable to prevent an undefined behaviour in the next activation
                        mNextState = GetState();
                    }
                };
            }
        }
    }
}