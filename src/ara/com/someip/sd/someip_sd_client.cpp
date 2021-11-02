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
                                              mTtlTimer(),
                                              mServiceNotseenState(&mTtlTimer),
                                              mServiceSeenState(&mTtlTimer),
                                              mInitialWaitState(
                                                  helper::SdClientState::InitialWaitPhase,
                                                  helper::SdClientState::RepetitionPhase,
                                                  helper::SdClientState::Stopped,
                                                  std::bind(&SomeIpSdClient::sendFind, this),
                                                  initialDelayMin,
                                                  initialDelayMax),
                                              mRepetitionState(
                                                  helper::SdClientState::RepetitionPhase,
                                                  helper::SdClientState::Stopped,
                                                  helper::SdClientState::Stopped,
                                                  std::bind(&SomeIpSdClient::sendFind, this),
                                                  repetitionMax,
                                                  repetitionBaseDelay),
                                              mServiceReadyState(&mTtlTimer),
                                              mStoppedState(&mTtlTimer),
                                              mFindServiceEntry{entry::ServiceEntry::CreateFindServiceEntry(serviceId)},
                                              mOfferingLock(mOfferingMutex, std::defer_lock),
                                              mStopOfferingLock(mStopOfferingMutex, std::defer_lock)
                {
                    this->StateMachine.Initialize(
                        {&mServiceNotseenState,
                         &mServiceSeenState,
                         &mInitialWaitState,
                         &mRepetitionState,
                         &mServiceReadyState,
                         &mStoppedState},
                        helper::SdClientState::ServiceNotSeen);

                    mFindServieMessage.AddEntry(&mFindServiceEntry);

                    auto _receiver =
                        std::bind(
                            &SomeIpSdClient::receiveSdMessage,
                            this,
                            std::placeholders::_1);
                    this->CommunicationLayer->SetReceiver(_receiver);
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
                    for (auto _entry : message.Entries())
                    {
                        if (_entry->Type() == entry::EntryType::Offering)
                        {
                            if (auto _serviceEnty = dynamic_cast<entry::ServiceEntry *>(_entry))
                            {
                                // Compare service ID, instance ID, major version and minor version
                                bool _result =
                                    (_serviceEnty->ServiceId() == mFindServiceEntry.ServiceId()) &&
                                    (mFindServiceEntry.InstanceId() == entry::ServiceEntry::cAnyInstanceId ||
                                     _serviceEnty->InstanceId() == mFindServiceEntry.InstanceId()) &&
                                    (mFindServiceEntry.MajorVersion() == entry::Entry::cAnyMajorVersion ||
                                     _serviceEnty->MajorVersion() == mFindServiceEntry.MajorVersion()) &&
                                    (mFindServiceEntry.MinorVersion() == entry::ServiceEntry::cAnyMinorVersion ||
                                     _serviceEnty->MinorVersion() == mFindServiceEntry.MinorVersion());

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

                    _clientServiceState->ServiceOffered(ttl);
                    mOfferingConditionVariable.notify_one();
                }

                void SomeIpSdClient::onServiceOfferStopped()
                {
                    helper::SdClientState _state = GetState();

                    switch (_state)
                    {
                    case helper::SdClientState::ServiceSeen:
                        mServiceSeenState.ServiceStopped();
                        mStopOfferingConditionVariable.notify_one();
                        break;
                    case helper::SdClientState::ServiceReady:
                        mServiceReadyState.ServiceStopped();
                        mStopOfferingConditionVariable.notify_one();
                        break;
                    case helper::SdClientState::RepetitionPhase:
                        mServiceReadyState.ServiceStopped();
                        mStopOfferingConditionVariable.notify_one();
                        break;
                    }
                }

                void SomeIpSdClient::receiveSdMessage(SomeIpSdMessage &&message)
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

                void SomeIpSdClient::StopAgent(helper::SdClientState state)
                {
                    switch (state)
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

                bool SomeIpSdClient::TryWaitUntiServiceOffered(uint32_t timeout)
                {
                    mOfferingLock.lock();
                    std::cv_status _status =
                        mOfferingConditionVariable.wait_for(
                            mOfferingLock, std::chrono::seconds(timeout));
                    mOfferingLock.unlock();
                    bool _result = _status != std::cv_status::timeout;

                    return _result;
                }

                bool SomeIpSdClient::TryWaitUntiServiceOfferStopped(uint32_t timeout)
                {
                    mStopOfferingLock.lock();
                    std::cv_status _status =
                        mStopOfferingConditionVariable.wait_for(
                            mStopOfferingLock, std::chrono::seconds(timeout));
                    mStopOfferingLock.unlock();
                    bool _result = _status != std::cv_status::timeout;

                    return _result;
                }

                SomeIpSdClient::~SomeIpSdClient()
                {
                    Stop();
                    Join();
                }
            }
        }
    }
}