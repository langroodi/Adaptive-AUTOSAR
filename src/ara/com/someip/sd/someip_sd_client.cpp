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
                                              mServiceReadyState(&mTtlTimer, &mOfferingConditionVariable),
                                              mStoppedState(&mTtlTimer, &mStopOfferingConditionVariable),
                                              mOfferingLock(mOfferingMutex, std::defer_lock),
                                              mStopOfferingLock(mStopOfferingMutex, std::defer_lock),
                                              mValidState{true},
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
                        this->Future =
                            std::async(
                                std::launch::async,
                                &fsm::ServiceNotseenState::ServiceRequested,
                                &mServiceNotseenState);
                        break;
                    case helper::SdClientState::ServiceSeen:
                        this->Future =
                            std::async(
                                std::launch::async,
                                &fsm::ServiceSeenState::ServiceRequested,
                                &mServiceSeenState);
                        break;
                    }
                }

                void SomeIpSdClient::StopAgent()
                {
                    mTtlTimer.Cancel();
                    mServiceReadyState.ServiceNotRequested();
                    mStoppedState.ServiceNotRequested();
                }

                bool SomeIpSdClient::TryWaitUntiServiceOffered(int duration)
                {
                    bool _result;
                    helper::SdClientState _state = GetState();

                    if (_state == helper::SdClientState::ServiceReady ||
                        _state == helper::SdClientState::ServiceSeen)
                    {
                        // The service has been already offered, so the function should return immediately.
                        _result = true;
                    }
                    else
                    {
                        mOfferingLock.lock();
                        std::cv_status _status =
                            mOfferingConditionVariable.wait_for(
                                mOfferingLock, std::chrono::milliseconds(duration));
                        mOfferingLock.unlock();
                        _result = mValidState && (_status != std::cv_status::timeout);
                    }

                    return _result;
                }

                bool SomeIpSdClient::TryWaitUntiServiceOfferStopped(int duration)
                {
                    bool _result;
                    helper::SdClientState _state = GetState();

                    if (_state == helper::SdClientState::Stopped ||
                        _state == helper::SdClientState::ServiceNotSeen)
                    {
                        // Offering the service has been already stopped, so the function should return immediately.
                        _result = true;
                    }
                    else
                    {
                        mStopOfferingLock.lock();
                        std::cv_status _status =
                            mStopOfferingConditionVariable.wait_for(
                                mStopOfferingLock, std::chrono::milliseconds(duration));
                        mStopOfferingLock.unlock();
                        _result = mValidState && (_status != std::cv_status::timeout);
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

                    Stop();
                    Join();
                }
            }
        }
    }
}