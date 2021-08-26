#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include "../../../../src/ara/com/entry/eventgroup_entry.h"
#include "../../../../src/ara/com/option/ipv4_endpoint_option.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            TEST(EventgroupEntryTest, SubscribeEventFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0002;
                const uint8_t cMajorVersion = 0x03;
                const uint16_t cEventgroupId = 0x0004;
                const EntryType cType = EntryType::Subscribing;
                const uint32_t cTTL = 0x000000;

                auto _entry =
                    EventgroupEntry::CreateSubscribeEventEntry(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                EXPECT_EQ(_entry.ServiceId(), cServiceId);
                EXPECT_EQ(_entry.InstanceId(), cInstanceId);
                EXPECT_EQ(_entry.MajorVersion(), cMajorVersion);
                EXPECT_EQ(_entry.EventgroupId(), cEventgroupId);
                EXPECT_EQ(_entry.Type(), cType);
                EXPECT_GT(_entry.TTL(), cTTL);
            }

            TEST(EventgroupEntryTest, UbsubscribeEventFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0002;
                const uint8_t cMajorVersion = 0x03;
                const uint16_t cEventgroupId = 0x0004;
                const EntryType cType = EntryType::Subscribing;
                const uint32_t cTTL = 0x000000;

                auto _entry =
                    EventgroupEntry::CreateUnsubscribeEventEntry(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                EXPECT_EQ(_entry.ServiceId(), cServiceId);
                EXPECT_EQ(_entry.InstanceId(), cInstanceId);
                EXPECT_EQ(_entry.MajorVersion(), cMajorVersion);
                EXPECT_EQ(_entry.EventgroupId(), cEventgroupId);
                EXPECT_EQ(_entry.Type(), cType);
                EXPECT_EQ(_entry.TTL(), cTTL);
            }

            TEST(EventgroupEntryTest, AcknowledgeFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0002;
                const uint8_t cMajorVersion = 0x03;
                const uint16_t cEventgroupId = 0x0004;
                const EntryType cType = EntryType::Acknowledging;

                auto _subscribeEntry =
                    EventgroupEntry::CreateSubscribeEventEntry(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                auto _ackEntry =
                    EventgroupEntry::CreateAcknowledgeEntry(_subscribeEntry);

                EXPECT_EQ(_ackEntry.ServiceId(), _subscribeEntry.ServiceId());
                EXPECT_EQ(_ackEntry.InstanceId(), _subscribeEntry.InstanceId());
                EXPECT_EQ(_ackEntry.MajorVersion(), _subscribeEntry.MajorVersion());
                EXPECT_EQ(_ackEntry.EventgroupId(), _subscribeEntry.EventgroupId());
                EXPECT_EQ(_ackEntry.TTL(), _subscribeEntry.TTL());
                EXPECT_EQ(_ackEntry.Type(), cType);
            }

            TEST(EventgroupEntryTest, NegativeAcknowledgeFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0002;
                const uint8_t cMajorVersion = 0x03;
                const uint16_t cEventgroupId = 0x0004;
                const uint32_t cTTL = 0x000000;
                const EntryType cType = EntryType::Acknowledging;

                auto _subscribeEntry =
                    EventgroupEntry::CreateSubscribeEventEntry(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                auto _nackEntry =
                    EventgroupEntry::CreateNegativeAcknowledgeEntry(_subscribeEntry);

                EXPECT_EQ(_nackEntry.ServiceId(), _subscribeEntry.ServiceId());
                EXPECT_EQ(_nackEntry.InstanceId(), _subscribeEntry.InstanceId());
                EXPECT_EQ(_nackEntry.MajorVersion(), _subscribeEntry.MajorVersion());
                EXPECT_EQ(_nackEntry.EventgroupId(), _subscribeEntry.EventgroupId());
                EXPECT_EQ(_nackEntry.TTL(), cTTL);
                EXPECT_EQ(_nackEntry.Type(), cType);
            }

            TEST(EventgroupEntryTest, PayloadMethod)
            {
                const uint16_t cServiceId = 0x0123;
                const uint16_t cInstanceId = 0x4567;
                const uint8_t cMajorVersion = 0x89;
                const uint16_t cEventgroupId = 0xabcd;
                const EntryType cType = EntryType::Subscribing;

                auto _entry =
                    EventgroupEntry::CreateSubscribeEventEntry(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                const size_t cPayloadSize = 16;
                const std::array<uint8_t, cPayloadSize> cExpectedPayload =
                    {0x06, 0x00, 0x00, 0x00,
                     0x01, 0x23, 0x45, 0x67,
                     0x89, 0xff, 0xff, 0xff,
                     0x00, 0x80, 0xab, 0xcd};

                uint8_t _optionIndex = 0;
                auto _actualPayload = _entry.Payload(_optionIndex);

                bool _areEqual =
                    std::equal(
                        _actualPayload.begin(),
                        _actualPayload.end(),
                        cExpectedPayload.begin());

                EXPECT_TRUE(_areEqual);
            }

            TEST(EventgroupEntryTest, AddOption)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0002;
                const uint8_t cMajorVersion = 0x03;
                const uint16_t cEventgroupId = 0x0004;

                const bool cDiscardable = false;
                const helper::Ipv4Address cIpAddress(224, 0, 0, 0);
                const uint16_t cPort = 8080;

                auto _subscribeEntry =
                    EventgroupEntry::CreateSubscribeEventEntry(
                        cServiceId, cInstanceId, cMajorVersion, cEventgroupId);

                auto _ackEntry =
                    EventgroupEntry::CreateAcknowledgeEntry(_subscribeEntry);

                auto _option =
                    option::Ipv4EndpointOption::CreateMulticastEndpoint(
                        cDiscardable, cIpAddress, cPort);

                EXPECT_THROW(
                    _subscribeEntry.AddFirstOption(&_option), std::invalid_argument);
                EXPECT_THROW(
                    _subscribeEntry.AddSecondOption(&_option), std::invalid_argument);
                EXPECT_NO_THROW(
                    _ackEntry.AddFirstOption(&_option));
                EXPECT_THROW(
                    _ackEntry.AddSecondOption(&_option), std::invalid_argument);
            }
        }
    }
}