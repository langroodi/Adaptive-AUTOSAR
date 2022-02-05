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

                void SomeIpSdClient::onOfferChanged(uint32_t ttl)
                {
                    mTtlTimer.SetOffered(ttl);
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
                            onOfferChanged(_ttl);
                        }
                    }
                }

                void SomeIpSdClient::StartAgent(helper::SdClientState state)
                {
                    mTtlTimer.SetRequested(true);

                    // Set the future if has not been set already
                    if (!Future.valid())
                    {
                        this->Future =
                            std::async(
                                std::launch::async,
                                &fsm::ServiceNotseenState::RequestService,
                                &mServiceNotseenState);
                    }
                }

                void SomeIpSdClient::StopAgent()
                {
                    if (!mValidState)
                    {
                        // Dispose the entry point state to stop the service offer monitoring
                        mServiceNotseenState.Dispose();
                        // Dispose the TTL timer to singal all the states for stopping immediately
                        mTtlTimer.Dispose();
                    }

                    // Send a synchronized cancel signal to all the state
                    mTtlTimer.SetRequested(false);
                }

                bool SomeIpSdClient::TryWaitUntiServiceOffered(int duration)
                {
                    bool _result;

                    if (mValidState)
                    {
                        std::cv_status _cvStatus;

                        mOfferingLock.lock();
                        if (duration > 0)
                        {
                            _cvStatus = mOfferingConditionVariable.wait_for(
                                mOfferingLock, std::chrono::milliseconds(duration));
                        }
                        else
                        {
                            mOfferingConditionVariable.wait(mOfferingLock);
                            _cvStatus = std::cv_status::no_timeout;
                        }
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
                        std::cv_status _cvStatus;

                        mStopOfferingLock.lock();
                        if (duration > 0)
                        {
                            _cvStatus =
                                mStopOfferingConditionVariable.wait_for(
                                    mStopOfferingLock, std::chrono::milliseconds(duration));
                        }
                        else
                        {
                            mStopOfferingConditionVariable.wait(mStopOfferingLock);
                            _cvStatus = std::cv_status::no_timeout;
                        }

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

                    Stop();
                    Join();
                }
            }
        }
    }
}