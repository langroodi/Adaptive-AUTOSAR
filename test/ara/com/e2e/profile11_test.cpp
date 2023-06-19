#include <gtest/gtest.h>
#include "../../../../src/ara/com/e2e/profile11.h"

namespace ara
{
    namespace com
    {
        namespace e2e
        {
            TEST(Profile11Test, InvalidProtection)
            {
                Profile11 _profile;

                const std::vector<uint8_t> cUnprotectedData;
                std::vector<uint8_t> _protectedData;

                const bool cResult{_profile.TryProtect(cUnprotectedData, _protectedData)};
                EXPECT_FALSE(cResult);
            }

            TEST(Profile11Test, ValidProtection)
            {
                Profile11 _profile;

                const size_t cCrcOffset{0};
                const uint8_t cExpectedResult{0x9f};

                const std::vector<uint8_t> cUnprotectedData{0x12, 0x34, 0x56, 0x78};
                std::vector<uint8_t> _protectedData;

                const bool cResult{_profile.TryProtect(cUnprotectedData, _protectedData)};
                EXPECT_TRUE(cResult);

                const uint8_t cActualResult{_protectedData.at(cCrcOffset)};
                EXPECT_EQ(cActualResult, cExpectedResult);
            }

            TEST(Profile11Test, NoNewDataCheck)
            {
                Profile11 _profile;

                const CheckStatusType cExpectedResult{CheckStatusType::kNoNewData};
                const std::vector<uint8_t> cProtectedData;

                const CheckStatusType cActualResult{_profile.Check(cProtectedData)};
                EXPECT_EQ(cActualResult, cExpectedResult);
            }

            TEST(Profile11Test, WrongCrcCheck)
            {
                Profile11 _profile;

                const CheckStatusType cExpectedResult{CheckStatusType::kWrongCrc};
                const std::vector<uint8_t> cProtectedData{0x00, 0xf1, 0x12, 0x34, 0x56, 0x78};

                const CheckStatusType cActualResult{_profile.Check(cProtectedData)};
                EXPECT_EQ(cActualResult, cExpectedResult);
            }

            TEST(Profile11Test, RepeatedCheck)
            {
                Profile11 _profile;

                const CheckStatusType cExpectedResult{CheckStatusType::kRepeated};
                const std::vector<uint8_t> cProtectedData{0xf5, 0xf0, 0x12, 0x34, 0x56, 0x78};

                const CheckStatusType cActualResult{_profile.Check(cProtectedData)};
                EXPECT_EQ(cActualResult, cExpectedResult);
            }

            TEST(Profile11Test, WrongSequenceCheckScenario)
            {
                Profile11 _profile;

                const CheckStatusType cFirstExpectedResult{CheckStatusType::kOk};
                const std::vector<uint8_t> cFirstProtectedData{0xf5, 0xf0, 0x12, 0x34, 0x56, 0x78};

                const CheckStatusType cLastExpectedResult{CheckStatusType::kWrongSequence};
                const std::vector<uint8_t> cLastProtectedData{0x9f, 0xf1, 0x12, 0x34, 0x56, 0x78};

                const CheckStatusType cFirstActualResult{_profile.Check(cLastProtectedData)};
                EXPECT_EQ(cFirstActualResult, cFirstExpectedResult);

                const CheckStatusType cLastActualResult{_profile.Check(cFirstProtectedData)};
                EXPECT_EQ(cLastActualResult, cLastExpectedResult);
            }
        }
    }
}