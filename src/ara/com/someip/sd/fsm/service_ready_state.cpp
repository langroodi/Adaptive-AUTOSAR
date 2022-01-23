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
                                                                               mConditionVariable{conditionVariable},
                                                                               mNextState{helper::SdClientState::ServiceReady}
                    {
                    }

                    void ServiceReadyState::waitForCountdown()
                    {
                        // Wait while the next state equals to the current state
                        while (mNextState == GetState())
                        {
                            bool _timeout = !Timer->Wait();

                            if (_timeout)
                            {
                                // TTL is expired while both client and server are up.
                                mNextState = helper::SdClientState::InitialWaitPhase;
                            }
                        }
                    }

                    void ServiceReadyState::Activate(helper::SdClientState previousState)
                    {
                        // Notify the condition variable that the service has been offered
                        mConditionVariable->notify_one();

                        waitForCountdown();
                        Transit(mNextState);
                    }

                    void ServiceReadyState::ServiceNotRequested() noexcept
                    {
                        // Only set the next state if it has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            // Client is not requested anymore
                            mNextState = helper::SdClientState::ServiceSeen;
                        }
                    }

                    void ServiceReadyState::ServiceOffered(uint32_t ttl) noexcept
                    {
                        // Only set the TTL timer if the next state has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            // Just reset the TTL, and stay in the same state
                            // because the client is still requested
                            Timer->Set(ttl);
                        }
                    }

                    void ServiceReadyState::ServiceStopped() noexcept
                    {
                        // Only set the next state if it has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            // Server is stopped while the client is still requested
                            mNextState = helper::SdClientState::Stopped;
                            Timer->Cancel();
                        }
                    }

                    void ServiceReadyState::Deactivate(helper::SdClientState nextState)
                    {
                        // Reset the next state variable to prevent an undefined behaviour in the next activation
                        mNextState = GetState();
                    }
                }
            }
        }
    }
}