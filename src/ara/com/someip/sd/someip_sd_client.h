#ifndef SOMEIP_SD_CLIENT
#define SOMEIP_SD_CLIENT

#include "../../helper/ipv4_address.h"
#include "../../helper/ttl_timer.h"
#include "../../helper/finite_state_machine.h"
#include "../../helper/network_layer.h"
#include "../../entry/service_entry.h"
#include "./fsm/service_notseen_state.h"
#include "./fsm/service_seen_state.h"
#include "./fsm/initial_wait_state.h"
#include "./fsm/repetition_state.h"
#include "./fsm/service_ready_state.h"
#include "./fsm/stopped_state.h"
#include "./someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery client
                class SomeIpSdClient
                {
                private:
                    helper::NetworkLayer<SomeIpSdMessage> *mNetworkLayer;
                    helper::TtlTimer mTtlTimer;
                    fsm::ServiceNotseenState mServiceNotseenState;
                    fsm::ServiceSeenState mServiceSeenState;
                    fsm::InitialWaitState<helper::SdClientState> mInitialWaitState;
                    fsm::RepetitionState<helper::SdClientState> mRepetitionState;
                    fsm::ServiceReadyState mServiceReadyState;
                    fsm::StoppedState mStoppedState;
                    helper::FiniteStateMachine<helper::SdClientState> mFiniteStateMachine;
                    entry::ServiceEntry mFindServiceEntry;
                    SomeIpSdMessage mFindServieMessage;

                    void sendFind();
                    bool matchRequestedService(
                        const SomeIpSdMessage &message, uint32_t &ttl) const;
                    void onServiceOffered(uint32_t ttl);
                    void onServiceOfferStopped();
                    void receiveSdMessage(SomeIpSdMessage &&message);

                public:
                    SomeIpSdClient() = delete;
                    ~SomeIpSdClient() noexcept = default;

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
                        int repetitionBaseDelay,
                        uint32_t repetitionMax);

                    /// @brief Start requesting the service
                    void Start();

                    /// @brief Stop requesting the service
                    void Stop();
                };
            }
        }
    }
}

#endif