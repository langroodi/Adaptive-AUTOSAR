#include "./service_ready_state.h"

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
                    ServiceReadyState::ServiceReadyState(
                        helper::TtlTimer *ttlTimer,
                        std::condition_variable *conditionVariable) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::ServiceReady),
                                                                               ClientServiceState(ttlTimer),
                                                                               mConditionVariable{conditionVariable}
                    {
                    }

                    helper::SdClientState ServiceReadyState::waitForNextState()
                    {
                        bool _expired = false;

                        while (Timer->GetRequested() && Timer->GetOffered())
                        {
                            _expired = Timer->WaitForExpiration();

                            if (_expired)
                            {
                                break;
                            }
                        }

                        if (!Timer->GetRequested())
                        {
                            return helper::SdClientState::ServiceSeen;
                        }
                        else if (_expired)
                        {
                            return helper::SdClientState::InitialWaitPhase;
                        }
                        else
                        {
                            return helper::SdClientState::Stopped;
                        }
                    }

                    void ServiceReadyState::Activate(helper::SdClientState previousState)
                    {
                        // Notify the condition variable that the service has been offered
                        mConditionVariable->notify_one();
                        helper::SdClientState _nextState = waitForNextState();
                        Transit(_nextState);
                    }

                    void ServiceReadyState::Deactivate(helper::SdClientState nextState)
                    {
                    }
                }
            }
        }
    }
}