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
                        helper::TtlTimer *ttlTimer) noexcept : helper::MachineState<SdClientState>(SdClientState::ServiceReady),
                                                               mTtlTimer{ttlTimer}
                    {
                    }

                    void ServiceReadyState::onTimerExpired()
                    {
                        Transit(SdClientState::InitialWaitPhase);
                    }

                    void ServiceReadyState::Activate(SdClientState previousState)
                    {
                        auto _callback =
                            std::bind(&ServiceReadyState::onTimerExpired, this);
                        mTtlTimer->SetExpirationCallback(_callback);
                    }

                    void ServiceReadyState::ServiceNotRequested()
                    {
                        Transit(SdClientState::ServiceSeen);
                    }

                    void ServiceReadyState::ServiceStopped()
                    {
                        mTtlTimer->Cancel();
                        Transit(SdClientState::Stopped);
                    }

                    void ServiceReadyState::Deactivate(SdClientState nextState)
                    {
                        mTtlTimer->ResetExpirationCallback();
                    }

                    ServiceReadyState::~ServiceReadyState()
                    {
                        mTtlTimer->ResetExpirationCallback();
                    }
                }
            }
        }
    }
}