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
                        helper::TtlTimer *ttlTimer) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::ServiceReady),
                        mTimer{ttlTimer}
                    {
                    }

                    void ServiceReadyState::onTimerExpired()
                    {
                        Transit(helper::SdClientState::InitialWaitPhase);
                    }

                    void ServiceReadyState::Activate(helper::SdClientState previousState)
                    {
                        auto _callback =
                            std::bind(&ServiceReadyState::onTimerExpired, this);
                        mTimer->SetExpirationCallback(_callback);
                    }

                    void ServiceReadyState::ServiceNotRequested()
                    {
                        Transit(helper::SdClientState::ServiceSeen);
                    }

                    void ServiceReadyState::ServiceOffered(uint32_t ttl)
                    {
                        mTimer->Reset(ttl);
                    }

                    void ServiceReadyState::ServiceStopped()
                    {
                        mTimer->Cancel();
                        Transit(helper::SdClientState::Stopped);
                    }

                    void ServiceReadyState::Deactivate(helper::SdClientState nextState)
                    {
                        mTimer->ResetExpirationCallback();
                    }

                    ServiceReadyState::~ServiceReadyState()
                    {
                        mTimer->ResetExpirationCallback();
                    }
                }
            }
        }
    }
}