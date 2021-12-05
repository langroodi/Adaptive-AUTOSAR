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
                                                                               mRequested{true}
                    {
                    }

                    void StoppedState::Activate(helper::SdClientState previousState)
                    {
                        mActivated = true;

                        // Notify the condition variable that the service is not offered yet
                        mConditionVariable->notify_one();

                        if (!mRequested)
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
                            // Reset the requested flag
                            mRequested = false;
                        }
                    }

                    void StoppedState::ServiceOffered(uint32_t ttl)
                    {
                        Timer->Reset(ttl);
                        Transit(helper::SdClientState::ServiceReady);
                    }

                    void StoppedState::Deactivate(helper::SdClientState nextState)
                    {
                        // Set the requested to default
                        mRequested = true;
                        mActivated = false;
                    }
                };
            }
        }
    }
}