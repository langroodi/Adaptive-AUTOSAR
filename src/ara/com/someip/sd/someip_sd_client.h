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
                    ServiceNotSeen,       ///!< Service is not requested and not seen
                    ServiceSeen,          ///!< Service is not requsted, but seen
                    ServiceReady,         ///!< Service is ready
                    Stopped,              ///!< Service is stopped
                    InitialWaitPhase,     ///!< Client service is in initial waiting phase
                    RepetitionPhase,      ///!< Client service is in repetition phase
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
                    const int mInitialDelayMin;
                    const int mInitialDelayMax;
                    const int mRepetitionBaseDelay;
                    uint32_t mRepetitionCounter;
                    bool mServiceRequested;

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