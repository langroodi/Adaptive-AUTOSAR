#include "./someip_pubsub_client.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                SomeIpPubSubClient::SomeIpPubSubClient(
                    helper::NetworkLayer<sd::SomeIpSdMessage> *networkLayer,
                    uint8_t counter) : mSubscriptionLock(mSubscriptionMutex, std::defer_lock),
                                       mCommunicationLayer{networkLayer},
                                       mCounter{counter},
                                       mValidNotify{true}
                {
                    auto _receiver =
                        std::bind(
                            &SomeIpPubSubClient::onMessageReceived,
                            this,
                            std::placeholders::_1);

                    mCommunicationLayer->SetReceiver(this, _receiver);
                }

                void SomeIpPubSubClient::onMessageReceived(sd::SomeIpSdMessage &&message)
                {
                    for (auto &entry : message.Entries())
                    {
                        if (entry->Type() == entry::EntryType::Acknowledging)
                        {
                            bool _enqueued = mMessageBuffer.TryEnqueue(std::move(message));

                            if (_enqueued)
                            {
                                mSubscriptionConditionVariable.notify_one();
                            }
                        }
                    }
                }

                void SomeIpPubSubClient::Subscribe(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint16_t eventgroupId)
                {
                    sd::SomeIpSdMessage _message;

                    auto _entry{
                        entry::EventgroupEntry::CreateSubscribeEventEntry(
                            serviceId, instanceId, majorVersion, mCounter, eventgroupId)};
                    _message.AddEntry(std::move(_entry));

                    mCommunicationLayer->Send(_message);
                }

                void SomeIpPubSubClient::Unsubscribe(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint16_t eventgroupId)
                {
                    sd::SomeIpSdMessage _message;

                    auto _entry{
                        entry::EventgroupEntry::CreateUnsubscribeEventEntry(
                            serviceId, instanceId, majorVersion, mCounter, eventgroupId)};
                    _message.AddEntry(std::move(_entry));

                    mCommunicationLayer->Send(_message);
                }

                bool SomeIpPubSubClient::TryGetProcessedSubscription(
                    int duration,
                    sd::SomeIpSdMessage &message)
                {
                    bool _result;

                    if (mMessageBuffer.Empty())
                    {
                        mSubscriptionLock.lock();
                        std::cv_status _status =
                            mSubscriptionConditionVariable.wait_for(
                                mSubscriptionLock, std::chrono::milliseconds(duration));
                        mSubscriptionLock.unlock();
                        _result = mValidNotify && (_status != std::cv_status::timeout);
                    }
                    else
                    {
                        // There are still processed subscription messages in the buffer, so no need to wait.
                        _result = true;
                    }

                    // In the case of successful get, set the first processed subscription to the output argument
                    if (_result)
                    {
                        _result = mMessageBuffer.TryDequeue(message);
                    }

                    return _result;
                }

                SomeIpPubSubClient::~SomeIpPubSubClient()
                {
                    // Condition variable notifications are not valid anymore during destruction.
                    mValidNotify = false;
                    // Release the threads waiting for the condition variables before desctruction
                    mSubscriptionConditionVariable.notify_one();
                }
            }
        }
    }
}