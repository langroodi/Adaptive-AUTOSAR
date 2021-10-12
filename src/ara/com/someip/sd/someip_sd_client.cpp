#include <stdexcept>
#include "./someip_sd_client.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                SomeIpSdClient::SomeIpSdClient(
                    helper::Ipv4Address sdIpAddress,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    uint32_t repetitionMax,
                    uint16_t sdPort,
                    bool serviceRequested) : mSdIpAddress{sdIpAddress},
                                             mSdPort{sdPort},
                                             mTtlTimer(),
                                             mServiceNotseenState(&mTtlTimer),
                                             mServiceSeenState(&mTtlTimer),
                                             mInitialWaitState(
                                                 helper::SdClientState::InitialWaitPhase,
                                                 helper::SdClientState::RepetitionPhase,
                                                 helper::SdClientState::Stopped,
                                                 std::bind(SomeIpSdClient::sendFind, this),
                                                 initialDelayMin,
                                                 initialDelayMax),
                                             mRepetitionState(
                                                 helper::SdClientState::RepetitionPhase,
                                                 helper::SdClientState::Stopped,
                                                 helper::SdClientState::Stopped,
                                                 std::bind(SomeIpSdClient::sendFind, this),
                                                 repetitionMax,
                                                 repetitionBaseDelay),
                                             mServiceReadyState(&mTtlTimer),
                                             mStoppedState(&mTtlTimer),
                                             mFiniteStateMachine(
                                                 {&mServiceNotseenState,
                                                  &mServiceSeenState,
                                                  &mInitialWaitState,
                                                  &mRepetitionState,
                                                  &mServiceReadyState,
                                                  &mStoppedState},
                                                 serviceRequested ? helper::SdClientState::InitialWaitPhase : helper::SdClientState::ServiceNotSeen)
                {
                    if ((initialDelayMin < 0) ||
                        (initialDelayMax < 0) ||
                        (initialDelayMin < initialDelayMax))
                    {
                        throw std::invalid_argument(
                            "Invalid initial delay minimum and/or maximum.");
                    }
                }

                void SomeIpSdClient::sendFind()
                {
                    /// @todo Link with the network abstraction layer
                }

                void SomeIpSdClient::onServiceOffered(uint32_t ttl)
                {
                    auto _machineState = mFiniteStateMachine.GetMachineState();
                    auto _clientServiceState =
                        dynamic_cast<fsm::ClientServiceState *>(_machineState);

                    _clientServiceState->ServiceOffered(ttl);
                }

                void SomeIpSdClient::onServiceOfferStopped()
                {
                    helper::SdClientState _state = mFiniteStateMachine.GetState();

                    switch (_state)
                    {
                    case helper::SdClientState::ServiceSeen:
                        mServiceSeenState.ServiceStopped();
                        break;
                    case helper::SdClientState::ServiceReady:
                        mServiceReadyState.ServiceStopped();
                        break;
                    case helper::SdClientState::RepetitionPhase:
                        mServiceReadyState.ServiceStopped();
                        break;
                    }
                }

                void SomeIpSdClient::Start()
                {
                    helper::SdClientState _state = mFiniteStateMachine.GetState();

                    switch (_state)
                    {
                    case helper::SdClientState::ServiceNotSeen:
                        mServiceNotseenState.ServiceRequested();
                        break;
                    case helper::SdClientState::ServiceSeen:
                        mServiceSeenState.ServiceRequested();
                        break;
                    }
                }

                void SomeIpSdClient::Stop()
                {
                    helper::SdClientState _state = mFiniteStateMachine.GetState();
                    switch (_state)
                    {
                    case helper::SdClientState::InitialWaitPhase:
                    case helper::SdClientState::RepetitionPhase:
                        mTtlTimer.Cancel();
                        break;
                    case helper::SdClientState::ServiceReady:
                        mServiceReadyState.ServiceNotRequested();
                        break;
                    case helper::SdClientState::Stopped:
                        mStoppedState.ServiceNotRequested();
                        break;
                    }
                }
            }
        }
    }
}