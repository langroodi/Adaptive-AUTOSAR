#ifndef SOMEIP_SD_CLIENT
#define SOMEIP_SD_CLIENT

#include "../../helper/ipv4_address.h"
#include "../../helper/ttl_timer.h"
#include "../../helper/finite_state_machine.h"
#include "./fsm/service_notseen_state.h"
#include "./fsm/service_seen_state.h"
#include "./fsm/initial_wait_state.h"
#include "./fsm/repetition_state.h"
#include "./fsm/service_ready_state.h"
#include "./fsm/stopped_state.h"

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
                    static const uint16_t cDefaultSdPort = 30490;

                    const helper::Ipv4Address mSdIpAddress;
                    const uint16_t mSdPort;

                    helper::TtlTimer mTtlTimer;
                    fsm::ServiceNotseenState mServiceNotseenState;
                    fsm::ServiceSeenState mServiceSeenState;
                    fsm::InitialWaitState<helper::SdClientState> mInitialWaitState;
                    fsm::RepetitionState<helper::SdClientState> mRepetitionState;
                    fsm::ServiceReadyState mServiceReadyState;
                    fsm::StoppedState mStoppedState;
                    helper::FiniteStateMachine<helper::SdClientState> mFiniteStateMachine;

                    void sendFind();

                public:
                    SomeIpSdClient() = delete;
                    ~SomeIpSdClient() noexcept = default;

                    /// @brief Constructor
                    /// @param sdIpAddress Service discovery IP Address
                    /// @param initialDelayMin Minimum initial delay
                    /// @param initialDelayMax Maximum initial delay
                    /// @param repetitionBaseDelay Repetition phase delay
                    /// @param repetitionMax Maximum message count in the repetition phase
                    /// @param sdPort Service discovery port number
                    /// @param serviceRequested Indicates whether the service is requested right after construction or not
                    SomeIpSdClient(
                        helper::Ipv4Address sdIpAddress,
                        int initialDelayMin,
                        int initialDelayMax,
                        int repetitionBaseDelay,
                        uint32_t repetitionMax,
                        uint16_t sdPort = cDefaultSdPort,
                        bool serviceRequested = true);

                    /// @brief Determine whether the service is requested or not
                    /// @param requested True, to request the service, and false to stop requesting
                    void RequestService(bool requested) noexcept;
                };
            }
        }
    }
}

#endif