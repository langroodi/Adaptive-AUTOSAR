#include "./service_notseen_state.h"

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
                    ServiceNotseenState::ServiceNotseenState(
                        helper::TtlTimer *ttlTimer,
                        std::condition_variable *conditionVariable) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::ServiceNotSeen),
                                                                               ClientServiceState(ttlTimer),
                                                                               mConditionVariable{conditionVariable},
                                                                               mEverRequested{false},
                                                                               mNextState{helper::SdClientState::ServiceNotSeen}

                    {
                    }

                    void ServiceNotseenState::Activate(helper::SdClientState previousState)
                    {
                        mConditionVariable->notify_one();
                        
                        // If the sevice client has ever been requested, keep
                        // the state flow in the loop by waiting for the timer
                        if (mEverRequested)
                        {
                            Timer->WaitForSignal();
                            Transit(mNextState);
                        }
                    }

                    void ServiceNotseenState::ServiceRequested()
                    {
                        mEverRequested = true;
                        // Due to calling from a separate thread, it is safe to directly transit.
                        Transit(helper::SdClientState::InitialWaitPhase);
                    }

                    void ServiceNotseenState::ServiceOffered(uint32_t ttl) noexcept
                    {
                        mNextState = helper::SdClientState::ServiceSeen;
                        Timer->Set(ttl);
                    }

                    void ServiceNotseenState::ResetEverRequested() noexcept
                    {
                        mEverRequested = false;
                    }

                    void ServiceNotseenState::Deactivate(helper::SdClientState nextState)
                    {
                        // Reset the next state variable to prevent a dead-lock while destruction
                        mNextState = GetState();
                    }
                }
            }
        }
    }
}