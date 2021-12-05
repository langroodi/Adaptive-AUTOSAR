#ifndef SOMEIP_SD_SERVER
#define SOMEIP_SD_SERVER

#include <queue>
#include "../../helper/ipv4_address.h"
#include "../../entry/service_entry.h"
#include "../../option/ipv4_endpoint_option.h"
#include "./fsm/notready_state.h"
#include "./fsm/initial_wait_state.h"
#include "./fsm/repetition_state.h"
#include "./fsm/main_state.h"
#include "./someip_sd_agent.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery server
                class SomeIpSdServer : public SomeIpSdAgent<helper::SdServerState>
                {
                private:
                    std::queue<SomeIpSdMessage> mMessageBuffer;
                    fsm::NotReadyState mNotReadyState;
                    fsm::InitialWaitState<helper::SdServerState> mInitialWaitState;
                    fsm::RepetitionState<helper::SdServerState> mRepetitionState;
                    fsm::MainState mMainState;
                    SomeIpSdMessage mOfferServiceMessage;
                    SomeIpSdMessage mStopOfferMessage;
                    std::shared_ptr<entry::ServiceEntry> mOfferServiceEntry;
                    std::shared_ptr<entry::ServiceEntry> mStopOfferEntry;
                    std::shared_ptr<option::Ipv4EndpointOption> mEndpointOption;

                    void sendOffer();
                    bool matchOfferingService(const SomeIpSdMessage &message) const;
                    void receiveFind(SomeIpSdMessage &&message);
                    void onServiceStopped();

                protected:
                    void StartAgent(helper::SdServerState state) override;
                    void StopAgent() override;

                public:
                    SomeIpSdServer() = delete;

                    /// @brief Constructor
                    /// @param networkLayer Network communication abstraction layer
                    /// @param serviceId Service ID
                    /// @param instanceId Service instance ID
                    /// @param majorVersion Service major version
                    /// @param minorVersion Service minor version
                    /// @param ipAddress Service unicast endpoint IP Address
                    /// @param port Service unicast endpoint TCP port number
                    /// @param initialDelayMin Minimum initial delay
                    /// @param initialDelayMax Maximum initial delay
                    /// @param repetitionBaseDelay Repetition phase delay
                    /// @param cycleOfferDelay Cycle offer delay in the main phase
                    /// @param repetitionMax Maximum message count in the repetition phase
                    SomeIpSdServer(
                        helper::NetworkLayer<SomeIpSdMessage> *networkLayer,
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint32_t minorVersion,
                        helper::Ipv4Address ipAddress,
                        uint16_t port,
                        int initialDelayMin,
                        int initialDelayMax,
                        int repetitionBaseDelay,
                        int cycleOfferDelay,
                        uint32_t repetitionMax);

                    ~SomeIpSdServer() override;
                };
            }
        }
    }
}

#endif