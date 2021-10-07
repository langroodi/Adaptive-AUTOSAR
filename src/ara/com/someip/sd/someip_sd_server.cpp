#include <stdexcept>
#include "./someip_sd_server.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                SomeIpSdServer::SomeIpSdServer(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint32_t minorVersion,
                    helper::Ipv4Address sdIpAddress,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    int cycleOfferDelay,
                    uint32_t repetitionMax,
                    uint16_t sdPort,
                    bool serviceAvailable) : mServiceId{serviceId},
                                             mInstanceId{instanceId},
                                             mMajorVersion{majorVersion},
                                             mMinorVersion{minorVersion},
                                             mSdIpAddress{sdIpAddress},
                                             mSdPort{sdPort},
                                             mNotReadyState(),
                                             mInitialWaitState(
                                                 SdServerState::InitialWaitPhase,
                                                 SdServerState::RepetitionPhase,
                                                 SdServerState::NotReady,
                                                 std::bind(SomeIpSdServer::sendOffer, this),
                                                 initialDelayMin,
                                                 initialDelayMax),
                                             mRepetitionState(
                                                 SdServerState::RepetitionPhase,
                                                 SdServerState::MainPhase,
                                                 SdServerState::NotReady,
                                                 std::bind(SomeIpSdServer::sendOffer, this),
                                                 repetitionMax,
                                                 repetitionBaseDelay),
                                             mMainState(
                                                 std::bind(SomeIpSdServer::sendOffer, this),
                                                 cycleOfferDelay),
                                             mFiniteStateMachine(
                                                 {&mNotReadyState, &mInitialWaitState, &mRepetitionState, &mMainState},
                                                 serviceAvailable ? SdServerState::InitialWaitPhase : SdServerState::NotReady)

                {
                    if (repetitionBaseDelay < 0)
                    {
                        throw std::invalid_argument(
                            "Invalid repetition base delay.");
                    }

                    if (cycleOfferDelay < 0)
                    {
                        throw std::invalid_argument(
                            "Invalid cyclic offer delay.");
                    }

                    if ((initialDelayMin < 0) ||
                        (initialDelayMax < 0) ||
                        (initialDelayMin < initialDelayMax))
                    {
                        throw std::invalid_argument(
                            "Invalid initial delay minimum and/or maximum.");
                    }
                }

                void SomeIpSdServer::sendOffer()
                {
                    /// @todo Link with the network abstraction layer
                }

                void SomeIpSdServer::SetServiceAvailability(bool available) noexcept
                {
                    SdServerState _state = mFiniteStateMachine.GetState();

                    if (available && _state == SdServerState::NotReady)
                    {
                        mNotReadyState.ServiceActivated();
                    }
                    else if (!available)
                    {
                        switch (_state)
                        {
                        case SdServerState::InitialWaitPhase:
                            mInitialWaitState.ServiceStopped();
                            break;
                        case SdServerState::RepetitionPhase:
                            mRepetitionState.ServiceStopped();
                            break;

                        case SdServerState::MainPhase:
                            mMainState.ServiceStopped();
                            break;
                        }
                    }
                }
            }
        }
    }
}