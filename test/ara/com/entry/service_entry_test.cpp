#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include "../../../../src/ara/com/entry/service_entry.h"
#include "../../../../src/ara/com/option/loadbalancing_option.h"

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
                const uint8_t cMajorVersion = 0x04;
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
                const uint8_t cMajorVersion = 0x04;
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
                const uint8_t cMajorVersion = 0x04;
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

            TEST(ServiceEntryTest, PayloadMethod)
            {
                const uint16_t cServiceId = 0x1234;
                const uint32_t cTTL = 0xabcdef;
                const uint16_t cInstanceId = 0xfedc;
                const uint8_t cMajorVersion = 0xba;
                const uint32_t cMinorVersion = 0x87654321;
                const EntryType cType = EntryType::Finding;

                auto _entry =
                    ServiceEntry::CreateFindServiceEntry(
                        cServiceId, cTTL, cInstanceId, cMajorVersion, cMinorVersion);

                const size_t cPayloadSize = 16;
                const std::array<uint8_t, cPayloadSize> cExpectedPayload =
                    {0x00, 0x00, 0x00, 0x00,
                     0x12, 0x34, 0xfe, 0xdc,
                     0xba, 0xab, 0xcd, 0xef,
                     0x87, 0x65, 0x43, 0x21};

                uint8_t _optionIndex = 0;
                auto _actualPayload = _entry.Payload(_optionIndex);

                bool _areEqual =
                    std::equal(
                        _actualPayload.begin(),
                        _actualPayload.end(),
                        cExpectedPayload.begin());

                EXPECT_TRUE(_areEqual);
            }

            TEST(ServiceEntryTest, AddOption)
            {
                const uint16_t cServiceId = 0x0001;
                const uint32_t cTTL = 0x000002;
                const uint16_t cInstanceId = 0x0003;
                const uint8_t cMajorVersion = 0x04;
                const uint32_t cMinorVersion = 0x00000005;
                const EntryType cType = EntryType::Finding;

                const bool cDiscardable = false;
                const uint16_t cPriority = 1;
                const uint16_t cWeight = 2;

                auto _entry =
                    ServiceEntry::CreateFindServiceEntry(
                        cServiceId, cTTL, cInstanceId, cMajorVersion, cMinorVersion);

                option::LoadBalancingOption _option(cDiscardable, cPriority, cWeight);

                EXPECT_NO_THROW(_entry.AddFirstOption(&_option));
                EXPECT_THROW(_entry.AddSecondOption(&_option), std::invalid_argument);
            }
        }
    }
}