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
                    helper::Ipv4Address ipAddress,
                    uint16_t port,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    int cycleOfferDelay,
                    uint32_t repetitionMax,
                    uint16_t sdPort,
                    bool serviceAvailable) : mSdIpAddress{sdIpAddress},
                                             mSdPort{sdPort},
                                             mNotReadyState(),
                                             mInitialWaitState(
                                                 helper::SdServerState::InitialWaitPhase,
                                                 helper::SdServerState::RepetitionPhase,
                                                 helper::SdServerState::NotReady,
                                                 std::bind(SomeIpSdServer::sendOffer, this),
                                                 initialDelayMin,
                                                 initialDelayMax),
                                             mRepetitionState(
                                                 helper::SdServerState::RepetitionPhase,
                                                 helper::SdServerState::MainPhase,
                                                 helper::SdServerState::NotReady,
                                                 std::bind(SomeIpSdServer::sendOffer, this),
                                                 repetitionMax,
                                                 repetitionBaseDelay),
                                             mMainState(
                                                 std::bind(SomeIpSdServer::sendOffer, this),
                                                 cycleOfferDelay),
                                             mFiniteStateMachine(
                                                 {&mNotReadyState,
                                                  &mInitialWaitState,
                                                  &mRepetitionState,
                                                  &mMainState},
                                                 serviceAvailable ? helper::SdServerState::InitialWaitPhase : helper::SdServerState::NotReady),
                                             mOfferServiceEntry{
                                                 entry::ServiceEntry::CreateOfferServiceEntry(
                                                     serviceId,
                                                     instanceId,
                                                     majorVersion,
                                                     minorVersion)},
                                             mStopOfferEntry{
                                                 entry::ServiceEntry::CreateStopOfferEntry(
                                                     serviceId,
                                                     instanceId,
                                                     majorVersion,
                                                     minorVersion)},
                                             mEndpointOption{
                                                 option::Ipv4EndpointOption::CreateUnitcastEndpoint(
                                                     false,
                                                     ipAddress,
                                                     option::Layer4ProtocolType::Tcp,
                                                     port)}

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

                    mOfferServiceEntry.AddFirstOption(&mEndpointOption);
                    mOfferServiceMessage.AddEntry(&mOfferServiceEntry);

                    mStopOfferEntry.AddFirstOption(&mEndpointOption);
                    mStopOfferMessage.AddEntry(&mStopOfferEntry);
                }

                void SomeIpSdServer::sendOffer()
                {
                    mOfferServiceMessage.IncrementSessionId();
                    /// @todo Link with the network abstraction layer
                }

                void SomeIpSdServer::Start()
                {
                    helper::SdServerState _state = mFiniteStateMachine.GetState();

                    if (_state == helper::SdServerState::NotReady)
                    {
                        mNotReadyState.ServiceActivated();
                    }
                }

                void SomeIpSdServer::Stop()
                {
                    helper::SdServerState _state = mFiniteStateMachine.GetState();

                    switch (_state)
                    {
                    case helper::SdServerState::InitialWaitPhase:
                        mInitialWaitState.ServiceStopped();
                        break;
                    case helper::SdServerState::RepetitionPhase:
                        mRepetitionState.ServiceStopped();
                        break;
                    case helper::SdServerState::MainPhase:
                        mMainState.ServiceStopped();
                        break;
                    }
                }
            }
        }
    }
}