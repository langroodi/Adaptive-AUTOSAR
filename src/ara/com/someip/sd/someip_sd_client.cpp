#include <stdexcept>
#include "../../option/ipv4_endpoint_option.h"
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
                                              mServiceId{serviceId},
                                              mEndpointLock(mEndpointMutex, std::defer_lock)
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

                bool SomeIpSdClient::tryExtractOfferedEndpoint(
                    const SomeIpSdMessage &message,
                    std::string &ipAddress, uint16_t &port) const
                {
                    for (size_t i = 0; i < message.Entries().size(); ++i)
                    {
                        auto entry = message.Entries().at(i).get();

                        // Endpoints are end-up in the first options
                        for (size_t j = 0; entry->FirstOptions().size(); ++j)
                        {
                            auto option = entry->FirstOptions().at(j).get();

                            if (option->Type() == option::OptionType::IPv4Endpoint)
                            {
                                auto cEndpoint{
                                    dynamic_cast<const option::Ipv4EndpointOption *>(
                                        option)};

                                if (cEndpoint &&
                                    cEndpoint->L4Proto() == option::Layer4ProtocolType::Tcp)
                                {
                                    ipAddress = cEndpoint->IpAddress().ToString();
                                    port = cEndpoint->Port();

                                    return true;
                                }
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
                        bool _successful = matchRequestedService(message, _ttl);
                        if (_successful)
                        {
                            onOfferChanged(_ttl);

                            mEndpointLock.lock();
                            std::string _ipAddress;
                            uint16_t _port;
                            _successful =
                                tryExtractOfferedEndpoint(
                                    message, _ipAddress, _port);

                            if (_successful)
                            {
                                mOfferedIpAddress = _ipAddress;
                                mOfferedPort = _port;
                            }
                            mEndpointLock.unlock();
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

                bool SomeIpSdClient::TryGetOfferedEndpoint(
                    std::string &ipAddress, uint16_t &port)
                {
                    mEndpointLock.lock();
                    if (mOfferedIpAddress && mOfferedPort)
                    {
                        ipAddress = mOfferedIpAddress.Value();
                        port = mOfferedPort.Value();
                        mEndpointLock.unlock();
                        return true;
                    }
                    else
                    {
                        mEndpointLock.unlock();
                        return false;
                    }
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