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
                    uint32_t repetitionMax) : SomeIpSdAgent<helper::SdServerState>(networkLayer),
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
                    this->StateMachine.Initialize({&mNotReadyState,
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
                    this->CommunicationLayer->SetReceiver(_receiver);
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
                            this->CommunicationLayer->Send(mOfferServiceMessage);
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
                    this->CommunicationLayer->Send(mStopOfferMessage);
                    mStopOfferMessage.IncrementSessionId();
                }

                void SomeIpSdServer::StartAgent(helper::SdServerState state)
                {
                    if (state == helper::SdServerState::NotReady)
                    {
                        // Set the timer from a new thread with a random initial delay.
                        this->Future =
                            std::async(
                                std::launch::async,
                                &fsm::NotReadyState::ServiceActivated,
                                &mNotReadyState);
                    }
                }

                void SomeIpSdServer::StopAgent(helper::SdServerState state)
                {
                    mInitialWaitState.ServiceStopped();
                    mRepetitionState.ServiceStopped();
                    mMainState.ServiceStopped();
                }

                SomeIpSdServer::~SomeIpSdServer()
                {
                    Stop();
                    Join();
                }
            }
        }
    }
}