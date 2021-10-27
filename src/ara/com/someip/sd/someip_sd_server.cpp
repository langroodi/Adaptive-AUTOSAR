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
                    helper::NetworkLayer<SomeIpSdMessage> *networkLayer,
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint32_t minorVersion,
                    helper::Ipv4Address ipAddress,
                    uint16_t port,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    int cycleOfferDelay,
                    uint32_t repetitionMax) : mNetworkLayer{networkLayer},
                                              mNotReadyState(
                                                  std::bind(&SomeIpSdServer::onServiceStopped, this)),
                                              mInitialWaitState(
                                                  helper::SdServerState::InitialWaitPhase,
                                                  helper::SdServerState::RepetitionPhase,
                                                  helper::SdServerState::NotReady,
                                                  std::bind(&SomeIpSdServer::sendOffer, this),
                                                  initialDelayMin,
                                                  initialDelayMax),
                                              mRepetitionState(
                                                  helper::SdServerState::RepetitionPhase,
                                                  helper::SdServerState::MainPhase,
                                                  helper::SdServerState::NotReady,
                                                  std::bind(&SomeIpSdServer::sendOffer, this),
                                                  repetitionMax,
                                                  repetitionBaseDelay),
                                              mMainState(
                                                  std::bind(&SomeIpSdServer::sendOffer, this),
                                                  cycleOfferDelay),
                                              mFiniteStateMachine(),
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
                        (initialDelayMin > initialDelayMax))
                    {
                        throw std::invalid_argument(
                            "Invalid initial delay minimum and/or maximum.");
                    }

                    mFiniteStateMachine.Initialize({&mNotReadyState,
                                                    &mInitialWaitState,
                                                    &mRepetitionState,
                                                    &mMainState},
                                                   helper::SdServerState::NotReady);

                    mOfferServiceEntry.AddFirstOption(&mEndpointOption);
                    mOfferServiceMessage.AddEntry(&mOfferServiceEntry);

                    mStopOfferEntry.AddFirstOption(&mEndpointOption);
                    mStopOfferMessage.AddEntry(&mStopOfferEntry);

                    auto _receiver =
                        std::bind(
                            &SomeIpSdServer::receiveFind,
                            this,
                            std::placeholders::_1);
                    mNetworkLayer->SetReceiver(_receiver);
                }

                bool SomeIpSdServer::matchOfferingService(const SomeIpSdMessage &message) const
                {
                    // Iterate over all the message entry to search for the first Service Finding entry
                    for (auto _entry : message.Entries())
                    {
                        if (_entry->Type() == entry::EntryType::Finding)
                        {
                            if (auto _serviceEnty = dynamic_cast<entry::ServiceEntry *>(_entry))
                            {
                                // Compare service ID, instance ID, major version and minor version
                                bool _result =
                                    (_serviceEnty->ServiceId() == mOfferServiceEntry.ServiceId()) &&
                                    (_serviceEnty->InstanceId() == entry::ServiceEntry::cAnyInstanceId ||
                                     _serviceEnty->InstanceId() == mOfferServiceEntry.InstanceId()) &&
                                    (_serviceEnty->MajorVersion() == entry::Entry::cAnyMajorVersion ||
                                     _serviceEnty->MajorVersion() == mOfferServiceEntry.MajorVersion()) &&
                                    (_serviceEnty->MinorVersion() == entry::ServiceEntry::cAnyMinorVersion ||
                                     _serviceEnty->MinorVersion() == mOfferServiceEntry.MinorVersion());

                                return _result;
                            }
                        }
                    }

                    return false;
                }

                void SomeIpSdServer::sendOffer()
                {
                    if (!mMessageBuffer.empty())
                    {
                        SomeIpSdMessage _message = mMessageBuffer.front();

                        bool _matches = matchOfferingService(_message);
                        // Send the offer if the finding matches the service
                        if (_matches)
                        {
                            mNetworkLayer->Send(mOfferServiceMessage);
                            mOfferServiceMessage.IncrementSessionId();
                        }

                        // Remove the message from the buffer after the processing
                        mMessageBuffer.pop();
                    }
                }

                void SomeIpSdServer::receiveFind(SomeIpSdMessage &&message)
                {
                    mMessageBuffer.push(message);
                }

                void SomeIpSdServer::onServiceStopped()
                {
                    mNetworkLayer->Send(mStopOfferMessage);
                    mStopOfferMessage.IncrementSessionId();
                }

                void SomeIpSdServer::Start()
                {
                    helper::SdServerState _state = mFiniteStateMachine.GetState();

                    if (_state == helper::SdServerState::NotReady)
                    {
                        mNotReadyState.ServiceActivated();
                    }
                }

                helper::SdServerState SomeIpSdServer::GetState() const noexcept
                {
                    return mFiniteStateMachine.GetState();
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

                SomeIpSdServer::~SomeIpSdServer()
                {
                    Stop();
                }
            }
        }
    }
}