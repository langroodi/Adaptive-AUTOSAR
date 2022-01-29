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
                                                                               mDisposing{false},
                                                                               mEverRequested{false}

                    {
                    }

                    helper::SdClientState ServiceNotseenState::waitForNextState()
                    {
                        if (!Timer->GetRequested() && !Timer->GetOffered())
                        {
                            Timer->WaitForSignal();
                        }

                        if (Timer->GetRequested())
                        {
                            return helper::SdClientState::InitialWaitPhase;
                        }
                        else
                        {
                            return helper::SdClientState::ServiceSeen;
                        }
                    }

                    void ServiceNotseenState::Activate(helper::SdClientState previousState)
                    {
                        mConditionVariable->notify_one();

                        // If the sevice client has ever been requested and it is no disposing,
                        // keep the state flow in the loop by waiting for the timer
                        if (mEverRequested && !mDisposing)
                        {
                            helper::SdClientState _nextState = waitForNextState();
                            Transit(_nextState);
                        }
                    }

                    void ServiceNotseenState::RequestService()
                    {
                        mEverRequested = true;
                        // Due to calling from a separate thread, it is safe to directly transit.
                        Transit(helper::SdClientState::InitialWaitPhase);
                    }

                    void ServiceNotseenState::Dispose() noexcept
                    {
                        mDisposing = true;
                    }

                    void ServiceNotseenState::Deactivate(helper::SdClientState nextState)
                    {
                    }
                }
            }
        }
    }
}