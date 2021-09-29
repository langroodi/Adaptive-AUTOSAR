#ifndef SOMEIP_SD_CLIENT
#define SOMEIP_SD_CLIENT

#include "../../helper/ipv4_address.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief Service discovery client machine state
                enum class SdClientState
                {
                    ServiceNotSeen,       ///!< Client's service not requested and not seen
                    ServiceSeen,          ///!< Client's service not requsted, but seen
                    RequestedButNotReady, ///!< Client's service requested, but not ready
                    ServiceReady,         ///!< Client's service is ready
                    Stopped,              ///!< Client stopped
                    InitialWaitPhase,     ///!< Client is in initial waiting phase
                    InitialWaitTimerSet,  ///!< Client is in initial waiting timer set
                    RepetitionPhase,      ///!< Client is in repetition phase
                    RepetitionTimerSet    ///!< Client is in repetition timer set
                };

                /// @brief SOME/IP service discovery client
                class SomeIpSdClient
                {
                private:
                    static const uint16_t cDefaultSdPort = 30490;
                    static const SdClientState cInitialState = SdClientState::ServiceNotSeen;

                    const helper::Ipv4Address mSdIpAddress;
                    const uint16_t mSdPort;
                    SdClientState mState;
                    bool mLinkAvailable;
                    double mInitialDelay;
                    const double mRepetitionBaseDelay;
                    uint32_t mRepetitionCounter;

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
                    /// @param serviceAvailable Indicates whether the service is available right after construction or not
                    SomeIpSdClient(
                        helper::Ipv4Address sdIpAddress,
                        double initialDelayMin,
                        double initialDelayMax,
                        double repetitionBaseDelay,
                        uint32_t repetitionMax,
                        uint16_t sdPort = cDefaultSdPort,
                        bool serviceAvailable = true);
                };
            }
        }
    }
}

#endif