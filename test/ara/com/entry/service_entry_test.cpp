#include <gtest/gtest.h>
#include "../../../../src/ara/com/entry/service_entry.h"

namespace ara
{
    namespace com
    {
        namespace entry
        {
            TEST(ServiceEntryTest, FindServiceFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint32_t cTTL = 0x000002;
                const uint16_t cInstanceId = 0x0003;
                const uint8_t cMajorVersion = 0x004;
                const uint32_t cMinorVersion = 0x00000005;
                const EntryType cType = EntryType::Finding;

                auto _entry =
                    ServiceEntry::CreateFindServiceEntry(
                        cServiceId, cTTL, cInstanceId, cMajorVersion, cMinorVersion);

                EXPECT_EQ(_entry.ServiceId(), cServiceId);
                EXPECT_EQ(_entry.TTL(), cTTL);
                EXPECT_EQ(_entry.InstanceId(), cInstanceId);
                EXPECT_EQ(_entry.MajorVersion(), cMajorVersion);
                EXPECT_EQ(_entry.MinorVersion(), cMinorVersion);
                EXPECT_EQ(_entry.Type(), cType);
            }

            TEST(ServiceEntryTest, OfferServiceFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0003;
                const uint8_t cMajorVersion = 0x004;
                const uint32_t cMinorVersion = 0x00000005;
                const EntryType cType = EntryType::Offering;
                const uint32_t cTTL = 0x000000;

                auto _entry =
                    ServiceEntry::CreateOfferServiceEntry(
                        cServiceId, cInstanceId, cMajorVersion, cMinorVersion);

                EXPECT_EQ(_entry.ServiceId(), cServiceId);
                EXPECT_EQ(_entry.InstanceId(), cInstanceId);
                EXPECT_EQ(_entry.MajorVersion(), cMajorVersion);
                EXPECT_EQ(_entry.MinorVersion(), cMinorVersion);
                EXPECT_EQ(_entry.Type(), cType);
                EXPECT_GT(_entry.TTL(), cTTL);
            }

            TEST(ServiceEntryTest, StopOfferFactory)
            {
                const uint16_t cServiceId = 0x0001;
                const uint16_t cInstanceId = 0x0003;
                const uint8_t cMajorVersion = 0x004;
                const uint32_t cMinorVersion = 0x00000005;
                const EntryType cType = EntryType::Offering;
                const uint32_t cTTL = 0x000000;

                auto _entry =
                    ServiceEntry::CreateStopOfferEntry(
                        cServiceId, cInstanceId, cMajorVersion, cMinorVersion);

                EXPECT_EQ(_entry.ServiceId(), cServiceId);
                EXPECT_EQ(_entry.InstanceId(), cInstanceId);
                EXPECT_EQ(_entry.MajorVersion(), cMajorVersion);
                EXPECT_EQ(_entry.MinorVersion(), cMinorVersion);
                EXPECT_EQ(_entry.Type(), cType);
                EXPECT_EQ(_entry.TTL(), cTTL);
            }
        }
    }
}