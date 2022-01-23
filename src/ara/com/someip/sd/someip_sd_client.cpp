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
                    helper::NetworkLayer<SomeIpSdMessage> *networkLayer,
                    uint16_t serviceId,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    uint32_t repetitionMax) : SomeIpSdAgent<helper::SdClientState>(networkLayer),
                                              mValidState{true},
                                              mServiceNotseenState(&mTtlTimer, &mStopOfferingConditionVariable),
                                              mServiceSeenState(&mTtlTimer, &mOfferingConditionVariable),
                                              mInitialWaitState(
                                                  &mTtlTimer,
                                                  std::bind(&SomeIpSdClient::sendFind, this),
                                                  initialDelayMin,
                                                  initialDelayMax),
                                              mRepetitionState(
                                                  &mTtlTimer,
                                                  std::bind(&SomeIpSdClient::sendFind, this),
                                                  repetitionMax,
                                                  repetitionBaseDelay),
                                              mStoppedState(&mTtlTimer, &mStopOfferingConditionVariable),
                                              mServiceReadyState(&mTtlTimer, &mOfferingConditionVariable),
                                              mOfferingLock(mOfferingMutex, std::defer_lock),
                                              mStopOfferingLock(mStopOfferingMutex, std::defer_lock),
                                              mServiceId{serviceId}
                {
                    this->StateMachine.Initialize(
                        {&mServiceNotseenState,
                         &mServiceSeenState,
                         &mInitialWaitState,
                         &mRepetitionState,
                         &mServiceReadyState,
                         &mStoppedState},
                        helper::SdClientState::ServiceNotSeen);

                    auto _findServiceEntry{entry::ServiceEntry::CreateFindServiceEntry(serviceId)};
                    mFindServieMessage.AddEntry(std::move(_findServiceEntry));

                    auto _receiver =
                        std::bind(
                            &SomeIpSdClient::receiveSdMessage,
                            this,
                            std::placeholders::_1);
                    this->CommunicationLayer->SetReceiver(this, _receiver);
                }

                void SomeIpSdClient::sendFind()
                {
                    this->CommunicationLayer->Send(mFindServieMessage);
                    mFindServieMessage.IncrementSessionId();
                }

                bool SomeIpSdClient::matchRequestedService(
                    const SomeIpSdMessage &message, uint32_t &ttl) const
                {
                    // Iterate over all the message entry to search for the first Service Offering entry
                    for (auto &_entry : message.Entries())
                    {
                        if (_entry->Type() == entry::EntryType::Offering)
                        {
                            if (auto _serviceEnty = dynamic_cast<entry::ServiceEntry *>(_entry.get()))
                            {
                                // Compare service ID
                                bool _result = _serviceEnty->ServiceId() == mServiceId;

                                if (_result)
                                {
                                    ttl = _serviceEnty->TTL();
                                }

                                return _result;
                            }
                        }
                    }

                    return false;
                }

                void SomeIpSdClient::onServiceOffered(uint32_t ttl)
                {
                    auto _machineState = this->StateMachine.GetMachineState();

                    auto _clientServiceState =
                        dynamic_cast<fsm::ClientServiceState *>(_machineState);
                    if (_clientServiceState)
                    {
                        _clientServiceState->ServiceOffered(ttl);
                    }
                }

                void SomeIpSdClient::onServiceOfferStopped()
                {
                    helper::SdClientState _state = GetState();

                    switch (_state)
                    {
                    case helper::SdClientState::ServiceSeen:
                        mServiceSeenState.ServiceStopped();
                        break;
                    case helper::SdClientState::ServiceReady:
                        mServiceReadyState.ServiceStopped();
                        break;
                    case helper::SdClientState::InitialWaitPhase:
                        mInitialWaitState.ServiceStopped();
                        break;
                    case helper::SdClientState::RepetitionPhase:
                        mRepetitionState.ServiceStopped();
                        break;
                    }
                }

                void SomeIpSdClient::receiveSdMessage(SomeIpSdMessage &&message)
                {
                    // While destruction, ignore communication layer received messages
                    if (mValidState)
                    {
                        uint32_t _ttl;
                        bool _matches = matchRequestedService(message, _ttl);
                        if (_matches)
                        {
                            // TTL determines the Offering or Stop Offering message
                            if (_ttl > 0)
                            {
                                onServiceOffered(_ttl);
                            }
                            else
                            {
                                onServiceOfferStopped();
                            }
                        }
                    }
                }

                void SomeIpSdClient::StartAgent(helper::SdClientState state)
                {
                    switch (state)
                    {
                    case helper::SdClientState::ServiceNotSeen:
                        // First, ensure that the previous thread is gracefully finished
                        mTtlTimer.Cancel();
                        Join();

                        this->Future =
                            std::async(
                                std::launch::async,
                                &fsm::ServiceNotseenState::ServiceRequested,
                                &mServiceNotseenState);
                        break;
                    case helper::SdClientState::ServiceSeen:
                        mServiceSeenState.ServiceRequested();
                        break;
                    }
                }

                void SomeIpSdClient::StopAgent()
                {
                    mServiceReadyState.ServiceNotRequested();
                    mStoppedState.ServiceNotRequested();

                    if (mValidState)
                    {
                        // Send a synchronized cancel signal to all the state
                        mTtlTimer.Cancel();
                    }
                    else
                    {
                        // Send a synchronized zero-TTL set signal to all the state for stopping immediately
                        mTtlTimer.Set(0);
                    }
                }

                bool SomeIpSdClient::TryWaitUntiServiceOffered(int duration)
                {
                    bool _result;

                    if (mValidState)
                    {
                        mOfferingLock.lock();
                        std::cv_status _cvStatus =
                            mOfferingConditionVariable.wait_for(
                                mOfferingLock, std::chrono::milliseconds(duration));
                        mOfferingLock.unlock();

                        // Get the state in case of being already in the desired state
                        // leads to a timeout while the service is already offered
                        helper::SdClientState _state = GetState();

                        _result =
                            mValidState && (_cvStatus == std::cv_status::no_timeout ||
                                            _state == helper::SdClientState::ServiceReady ||
                                            _state == helper::SdClientState::ServiceSeen);
                    }
                    else
                    {
                        _result = false;
                    }

                    return _result;
                }

                bool SomeIpSdClient::TryWaitUntiServiceOfferStopped(int duration)
                {
                    bool _result;

                    if (mValidState)
                    {
                        mStopOfferingLock.lock();
                        std::cv_status _cvStatus =
                            mStopOfferingConditionVariable.wait_for(
                                mStopOfferingLock, std::chrono::milliseconds(duration));
                        mStopOfferingLock.unlock();

                        // Get the state in case of being already in the desired state
                        // leads to a timeout while the service offering is already stopped
                        helper::SdClientState _state = GetState();

                        _result =
                            mValidState && (_cvStatus == std::cv_status::no_timeout ||
                                            _state == helper::SdClientState::Stopped ||
                                            _state == helper::SdClientState::ServiceNotSeen);
                    }
                    else
                    {
                        _result = false;
                    }

                    return _result;
                }

                SomeIpSdClient::~SomeIpSdClient()
                {
                    // Client state is not valid anymore during destruction.
                    mValidState = false;
                    // Release the threads waiting for the condition variables before desctruction
                    mOfferingConditionVariable.notify_one();
                    mStopOfferingConditionVariable.notify_one();

                    mServiceNotseenState.ResetEverRequested();
                    Stop();
                    Join();
                }
            }
        }
    }
}