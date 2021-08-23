#include <gtest/gtest.h>
#include "../../../../src/ara/com/entry/eventgroup_entry.h"

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
                const uint8_t cMajorVersion = 0x003;
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
                const uint8_t cMajorVersion = 0x003;
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
                const uint8_t cMajorVersion = 0x003;
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
                const uint8_t cMajorVersion = 0x003;
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
        }
    }
}