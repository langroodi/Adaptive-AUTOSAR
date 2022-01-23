#include "./service_seen_state.h"

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
                    ServiceSeenState::ServiceSeenState(
                        helper::TtlTimer *ttlTimer,
                        std::condition_variable *conditionVariable) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::ServiceSeen),
                                                                               ClientServiceState(ttlTimer),
                                                                               mConditionVariable{conditionVariable},
                                                                               mNextState{helper::SdClientState::ServiceSeen}
                    {
                    }

                    void ServiceSeenState::waitForCountdown()
                    {
                        // Wait while the next state equals to the current state
                        while (mNextState == GetState())
                        {
                            bool _timeout = !Timer->Wait();

                            if (_timeout)
                            {
                                // TTL is expired.
                                mNextState = helper::SdClientState::ServiceNotSeen;
                            }
                        }
                    }

                    void ServiceSeenState::Activate(helper::SdClientState previousState)
                    {
                        mConditionVariable->notify_one();
                        waitForCountdown();
                        Transit(mNextState);
                    }

                    void ServiceSeenState::ServiceRequested()
                    {
                        // Only set the next state if it has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            mNextState = helper::SdClientState::ServiceReady;
                            Timer->Cancel();
                        }
                    }

                    void ServiceSeenState::ServiceOffered(uint32_t ttl) noexcept
                    {
                        // Only set the TTL timer if the next state has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            // Just reset the TTL, but stay in the same state
                            // because the client is still not requested
                            Timer->Set(ttl);
                        }
                    }

                    void ServiceSeenState::ServiceStopped() noexcept
                    {
                        // Only set the next state if it has not been set yet from the default value
                        if (mNextState == GetState())
                        {
                            mNextState = helper::SdClientState::ServiceNotSeen;
                            Timer->Cancel();
                        }
                    }

                    void ServiceSeenState::Deactivate(helper::SdClientState nextState)
                    {
                        // Reset the next state variable to prevent en undefined behaviour in the next activation
                        mNextState = GetState();
                    }
                }
            }
        }
    }
}