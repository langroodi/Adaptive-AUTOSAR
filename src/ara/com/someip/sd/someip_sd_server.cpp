#include <stdexcept>
#include <thread>
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
                                              mServiceId{serviceId},
                                              mInstanceId{instanceId},
                                              mMajorVersion{majorVersion},
                                              mMinorVersion{minorVersion}
                {
                    auto _offerServiceEntry{
                        entry::ServiceEntry::CreateOfferServiceEntry(
                            serviceId,
                            instanceId,
                            majorVersion,
                            minorVersion)};

                    auto _stopOfferEntry{
                        entry::ServiceEntry::CreateStopOfferEntry(
                            serviceId,
                            instanceId,
                            majorVersion,
                            minorVersion)};

                    auto _offerEndpointOption{
                        option::Ipv4EndpointOption::CreateUnitcastEndpoint(
                            false,
                            ipAddress,
                            option::Layer4ProtocolType::Tcp,
                            port)};

                    auto _stopOfferEndpointOption{
                        option::Ipv4EndpointOption::CreateUnitcastEndpoint(
                            false,
                            ipAddress,
                            option::Layer4ProtocolType::Tcp,
                            port)};

                    _offerServiceEntry->AddFirstOption(std::move(_offerEndpointOption));
                    mOfferServiceMessage.AddEntry(std::move(_offerServiceEntry));

                    _stopOfferEntry->AddFirstOption(std::move(_stopOfferEndpointOption));
                    mStopOfferMessage.AddEntry(std::move(_stopOfferEntry));

                    this->StateMachine.Initialize({&mNotReadyState,
                                                   &mInitialWaitState,
                                                   &mRepetitionState,
                                                   &mMainState},
                                                  helper::SdServerState::NotReady);

                    auto _receiver =
                        std::bind(
                            &SomeIpSdServer::receiveFind,
                            this,
                            std::placeholders::_1);
                    this->CommunicationLayer->SetReceiver(this, _receiver);
                }

                bool SomeIpSdServer::matchOfferingService(const SomeIpSdMessage &message) const
                {
                    // Iterate over all the message entry to search for the first Service Finding entry
                    for (auto &_entry : message.Entries())
                    {
                        if (_entry->Type() == entry::EntryType::Finding)
                        {
                            if (auto _serviceEnty = dynamic_cast<entry::ServiceEntry *>(_entry.get()))
                            {
                                // Compare service ID, instance ID, major version, and minor version
                                bool _result =
                                    (_serviceEnty->ServiceId() == mServiceId) &&
                                    (_serviceEnty->InstanceId() == entry::Entry::cAnyInstanceId ||
                                     _serviceEnty->InstanceId() == mInstanceId) &&
                                    (_serviceEnty->MajorVersion() == entry::Entry::cAnyMajorVersion ||
                                     _serviceEnty->MajorVersion() == mMajorVersion) &&
                                    (_serviceEnty->MinorVersion() == entry::ServiceEntry::cAnyMinorVersion ||
                                     _serviceEnty->MinorVersion() == mMinorVersion);

                                return _result;
                            }
                        }
                    }

                    return false;
                }

                void SomeIpSdServer::sendOffer()
                {
                    // Avoid starvation
                    while (!mMessageBuffer.Empty())
                    {
                        SomeIpSdMessage _message;
                        if (mMessageBuffer.TryDequeue(_message))
                        {
                            this->CommunicationLayer->Send(mOfferServiceMessage);
                            mOfferServiceMessage.IncrementSessionId();
                        }

                        std::this_thread::yield();
                    }
                }

                void SomeIpSdServer::receiveFind(SomeIpSdMessage &&message)
                {
                    bool _matches = matchOfferingService(message);
                    // Enqueue the offer if the finding message matches the service
                    if (_matches)
                    {
                        while (!mMessageBuffer.TryEnqueue(std::move(message)))
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        }
                    }
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

                void SomeIpSdServer::StopAgent()
                {
                    mInitialWaitState.ServiceStopped();
                    mRepetitionState.ServiceStopped();
                    mMainState.ServiceStopped();
                    Join();
                }

                SomeIpSdServer::~SomeIpSdServer()
                {
                    Stop();
                }
            }
        }
    }
}