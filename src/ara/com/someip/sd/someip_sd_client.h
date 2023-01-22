#ifndef SOMEIP_SD_CLIENT
#define SOMEIP_SD_CLIENT

#include "../../helper/ipv4_address.h"
#include "../../helper/ttl_timer.h"
#include "../../entry/service_entry.h"
#include "./fsm/service_notseen_state.h"
#include "./fsm/service_seen_state.h"
#include "./fsm/client_initial_wait_state.h"
#include "./fsm/client_repetition_state.h"
#include "./fsm/service_ready_state.h"
#include "./fsm/stopped_state.h"
#include "./someip_sd_agent.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery client
                class SomeIpSdClient : public SomeIpSdAgent<helper::SdClientState>
                {
                private:
                    helper::TtlTimer mTtlTimer;
                    bool mValidState;
                    std::mutex mOfferingMutex;
                    std::unique_lock<std::mutex> mOfferingLock;
                    std::condition_variable mOfferingConditionVariable;
                    std::mutex mStopOfferingMutex;
                    std::unique_lock<std::mutex> mStopOfferingLock;
                    std::condition_variable mStopOfferingConditionVariable;
                    fsm::ServiceNotseenState mServiceNotseenState;
                    fsm::ServiceSeenState mServiceSeenState;
                    fsm::ClientInitialWaitState mInitialWaitState;
                    fsm::ClientRepetitionState mRepetitionState;
                    fsm::StoppedState mStoppedState;
                    fsm::ServiceReadyState mServiceReadyState;
                    SomeIpSdMessage mFindServieMessage;
                    const uint16_t mServiceId;

                    void sendFind();
                    bool matchRequestedService(
                        const SomeIpSdMessage &message, uint32_t &ttl) const;
                    void onOfferChanged(uint32_t ttl);
                    void receiveSdMessage(SomeIpSdMessage &&message);

                protected:
                    void StartAgent(helper::SdClientState state) override;
                    void StopAgent() override;

                public:
                    SomeIpSdClient() = delete;

                    /// @brief Constructor
                    /// @param networkLayer Network communication abstraction layer
                    /// @param serviceId Server's service ID
                    /// @param initialDelayMin Minimum initial delay
                    /// @param initialDelayMax Maximum initial delay
                    /// @param repetitionBaseDelay Repetition phase delay
                    /// @param repetitionMax Maximum message count in the repetition phase
                    SomeIpSdClient(
                        helper::NetworkLayer<SomeIpSdMessage> *networkLayer,
                        uint16_t serviceId,
                        int initialDelayMin,
                        int initialDelayMax,
                        int repetitionBaseDelay = 30,
                        uint32_t repetitionMax = 3);

                    /// @brief Try to wait unitl the server offers the service
                    /// @param duration Waiting timeout in milliseconds
                    /// @returns True, if the service is offered before the timeout; otherwise false
                    /// @note Zero duration means wait until the service is offered.
                    bool TryWaitUntiServiceOffered(int duration);

                    /// @brief Try to wait unitl the server stops offering the service
                    /// @param duration Waiting timeout in milliseconds
                    /// @returns True, if the service offering is stopped before the timeout; otherwise false
                    /// @note Zero duration means wait until the service offering stops.
                    bool TryWaitUntiServiceOfferStopped(int duration);

                    ~SomeIpSdClient() override;
                };
            }
        }
    }
}

#endif