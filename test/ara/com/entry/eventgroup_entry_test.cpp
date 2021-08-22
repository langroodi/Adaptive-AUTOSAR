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
                const uint32_t cTTL = 0x00000000;

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
        }
    }
}