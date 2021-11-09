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
                                                                               mConditionVariable{conditionVariable}

                    {
                    }

                    void ServiceNotseenState::Activate(helper::SdClientState previousState)
                    {
                        mConditionVariable->notify_one();
                    }

                    void ServiceNotseenState::ServiceRequested()
                    {
                        Transit(helper::SdClientState::InitialWaitPhase);
                    }

                    void ServiceNotseenState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Set(ttl);
                        Transit(helper::SdClientState::ServiceSeen);
                    }

                    void ServiceNotseenState::Deactivate(helper::SdClientState nextState)
                    {
                        // Nothing to do on deactivation
                    }
                }
            }
        }
    }
}