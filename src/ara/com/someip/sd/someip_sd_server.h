#ifndef SOMEIP_SD_SERVER
#define SOMEIP_SD_SERVER

#include <queue>
#include "../../helper/ipv4_address.h"
#include "./someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief Service discovery server machine state
                enum class SdServerState
                {
                    NotReady,            ///!< Server is down
                    InitialWaitPhase,    ///!< Server is in initial waiting phase
                    InitialWaitTimerSet, ///!< Server is in initial waiting timer set
                    RepetitionPhase,     ///!< Server is in repetition phase
                    RepetitionTimerSet,  ///!< Server is in repetition timer set
                    MainPhase,           ///!< Server is in main phase
                    MainTimerSet         ///!< Server is in main timer set
                };

                /// @brief SOME/IP service discovery server
                class SomeIpSdServer
                {
                private:
                    static const uint16_t cDefaultSdPort = 30490;
                    static const SdServerState cInitialState = SdServerState::NotReady;

                    const uint16_t mServiceId;
                    const uint16_t mInstanceId;
                    const uint8_t mMajorVersion;
                    const uint32_t mMinorVersion;
                    const helper::Ipv4Address mSdIpAddress;
                    const uint16_t mSdPort;
                    SdServerState mState;
                    bool mServiceAvailable;
                    bool mLinkAvailable;
                    double mInitialDelay;
                    const double mRepetitionBaseDelay;
                    const double mCycleOfferDelay;
                    uint32_t mRepetitionCounter;
                    std::queue<SomeIpSdMessage> mMessageBuffer;

                public:
                    SomeIpSdServer() = delete;
                    ~SomeIpSdServer() noexcept = default;

                    /// @brief Constructor
                    /// @param serviceId Service ID
                    /// @param instanceId Service instance ID
                    /// @param majorVersion Service major version
                    /// @param minorVersion Service minor version
                    /// @param sdIpAddress Service discovery IP Address
                    /// @param initialDelayMin Minimum initial delay
                    /// @param initialDelayMax Maximum initial delay
                    /// @param repetitionBaseDelay Repetition phase delay
                    /// @param cycleOfferDelay Cycle offer delay in the main phase
                    /// @param repetitionMax Maximum message count in the repetition phase
                    /// @param sdPort Service discovery port number
                    /// @param serviceAvailable Indicates whether the service is available or not
                    SomeIpSdServer(
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint32_t minorVersion,
                        helper::Ipv4Address sdIpAddress,
                        double initialDelayMin,
                        double initialDelayMax,
                        double repetitionBaseDelay,
                        double cycleOfferDelay,
                        uint32_t repetitionMax,
                        uint16_t sdPort = cDefaultSdPort,
                        bool serviceAvailable = true);

                    /// @brief Set the service availability
                    /// @param avaible Indicates whether the service is available or not
                    void SetServiceAvailability(bool available) noexcept;
                };
            }
        }
    }
}

#endif