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
                                                                               mConditionVariable{conditionVariable}
                    {
                    }

                    void ServiceSeenState::onTimerExpired()
                    {
                        Transit(helper::SdClientState::ServiceNotSeen);
                    }

                    void ServiceSeenState::Activate(helper::SdClientState previousState)
                    {
                        mConditionVariable->notify_one();
                        auto _callback =
                            std::bind(&ServiceSeenState::onTimerExpired, this);
                        Timer->SetExpirationCallback(_callback);
                    }

                    void ServiceSeenState::ServiceRequested()
                    {
                        Transit(helper::SdClientState::ServiceReady);
                    }

                    void ServiceSeenState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                    }

                    void ServiceSeenState::ServiceStopped()
                    {
                        Timer->Cancel();
                        Transit(helper::SdClientState::ServiceNotSeen);
                    }

                    void ServiceSeenState::Deactivate(helper::SdClientState nextState)
                    {
                        Timer->ResetExpirationCallback();
                    }

                    ServiceSeenState::~ServiceSeenState()
                    {
                        ServiceStopped();
                    }
                }
            }
        }
    }
}