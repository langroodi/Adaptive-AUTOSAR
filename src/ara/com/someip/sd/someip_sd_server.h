#ifndef SOMEIP_SD_SERVER
#define SOMEIP_SD_SERVER

#include <queue>
#include "../../helper/ipv4_address.h"
#include "../../helper/finite_state_machine.h"
#include "../../entry/service_entry.h"
#include "../../option/ipv4_endpoint_option.h"
#include "./fsm/notready_state.h"
#include "./fsm/initial_wait_state.h"
#include "./fsm/repetition_state.h"
#include "./fsm/main_state.h"
#include "./someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery server
                class SomeIpSdServer
                {
                private:
                    static const uint16_t cDefaultSdPort = 30490;

                    const helper::Ipv4Address mSdIpAddress;
                    const uint16_t mSdPort;

                    std::queue<SomeIpSdMessage> mMessageBuffer;
                    helper::FiniteStateMachine<helper::SdServerState> mFiniteStateMachine;
                    fsm::NotReadyState mNotReadyState;
                    fsm::InitialWaitState<helper::SdServerState> mInitialWaitState;
                    fsm::RepetitionState<helper::SdServerState> mRepetitionState;
                    fsm::MainState mMainState;
                    SomeIpSdMessage mOfferServiceMessage;
                    SomeIpSdMessage mStopOfferMessage;
                    entry::ServiceEntry mOfferServiceEntry;
                    entry::ServiceEntry mStopOfferEntry;
                    option::Ipv4EndpointOption mEndpointOption;

                    void sendOffer();

                public:
                    SomeIpSdServer() = delete;
                    ~SomeIpSdServer() noexcept = default;

                    /// @brief Constructor
                    /// @param serviceId Service ID
                    /// @param instanceId Service instance ID
                    /// @param majorVersion Service major version
                    /// @param minorVersion Service minor version
                    /// @param sdIpAddress Service discovery IP Address
                    /// @param ipAddress Service unicast endpoint IP Address
                    /// @param port Service unicast endpoint TCP port number
                    /// @param initialDelayMin Minimum initial delay
                    /// @param initialDelayMax Maximum initial delay
                    /// @param repetitionBaseDelay Repetition phase delay
                    /// @param cycleOfferDelay Cycle offer delay in the main phase
                    /// @param repetitionMax Maximum message count in the repetition phase
                    /// @param sdPort Service discovery port number
                    /// @param serviceAvailable Indicates whether the service is available right after construction or not
                    SomeIpSdServer(
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint32_t minorVersion,
                        helper::Ipv4Address sdIpAddress,
                        helper::Ipv4Address ipAddress,
                        uint16_t port,
                        int initialDelayMin,
                        int initialDelayMax,
                        int repetitionBaseDelay,
                        int cycleOfferDelay,
                        uint32_t repetitionMax,
                        uint16_t sdPort = cDefaultSdPort,
                        bool serviceAvailable = true);

                    /// @brief Start the service discovery server
                    void Start();

                    /// @brief Stop the service discovery server
                    void Stop();
                };
            }
        }
    }
}

#endif