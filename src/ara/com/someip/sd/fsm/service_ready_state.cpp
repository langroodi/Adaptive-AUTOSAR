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
                                                                               mActivated{false},
                                                                               mRequested{true}
                    {
                    }

                    void ServiceReadyState::onTimerExpired()
                    {
                        Transit(helper::SdClientState::InitialWaitPhase);
                    }

                    void ServiceReadyState::Activate(helper::SdClientState previousState)
                    {
                        mActivated = true;

                        if (!mRequested)
                        {
                            Transit(helper::SdClientState::ServiceSeen);
                        }

                        // Notify the condition variable that the service has been offered
                        mConditionVariable->notify_one();

                        auto _callback =
                            std::bind(&ServiceReadyState::onTimerExpired, this);
                        Timer->SetExpirationCallback(_callback);
                    }

                    void ServiceReadyState::ServiceNotRequested()
                    {
                        if (mActivated)
                        {
                            Transit(helper::SdClientState::ServiceSeen);
                        }
                        else
                        {
                            // Reset the requested flag
                            mRequested = false;
                        }
                    }

                    void ServiceReadyState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                    }

                    void ServiceReadyState::ServiceStopped()
                    {
                        Timer->Cancel();
                        Transit(helper::SdClientState::Stopped);
                    }

                    void ServiceReadyState::Deactivate(helper::SdClientState nextState)
                    {
                        Timer->ResetExpirationCallback();
                        
                        // Set the requested flag to default
                        mRequested = true;
                        mActivated = false;
                    }

                    ServiceReadyState::~ServiceReadyState()
                    {
                        Timer->ResetExpirationCallback();
                    }
                }
            }
        }
    }
}