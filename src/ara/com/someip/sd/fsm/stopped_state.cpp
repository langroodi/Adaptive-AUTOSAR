#include "./stopped_state.h"

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
                    StoppedState::StoppedState(
                        helper::TtlTimer *ttlTimer,
                        std::condition_variable *conditionVariable) noexcept : helper::MachineState<helper::SdClientState>(helper::SdClientState::Stopped),
                                                                               ClientServiceState(ttlTimer),
                                                                               mConditionVariable{conditionVariable},
                                                                               mActivated{false},
                                                                               mClientRequested{true}
                    {
                    }

                    void StoppedState::Activate(helper::SdClientState previousState)
                    {
                        mActivated = true;

                        // Notify the condition variable that the service is not offered yet
                        mConditionVariable->notify_one();

                        if (!mClientRequested)
                        {
                            Transit(helper::SdClientState::ServiceNotSeen);
                        }
                    }

                    void StoppedState::ServiceNotRequested()
                    {
                        if (mActivated)
                        {
                            Transit(helper::SdClientState::ServiceNotSeen);
                        }
                        else
                        {
                            // Reset the client requested flag
                            mClientRequested = false;
                        }
                    }

                    void StoppedState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                        Transit(helper::SdClientState::ServiceReady);
                    }

                    void StoppedState::Deactivate(helper::SdClientState nextState)
                    {
                        // Set the client requested to default
                        mClientRequested = true;
                        mActivated = false;
                    }
                };
            }
        }
    }
}