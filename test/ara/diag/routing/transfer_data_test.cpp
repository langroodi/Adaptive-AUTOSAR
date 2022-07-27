#include "./testable_uds_service.h"
#include "../../../../src/ara/diag/routing/transfer_data.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            class TransferDataTest : public TestableUdsService
            {
            private:
                static core::InstanceSpecifier mSpecifier;

            protected:
                TransferData Service;

            public:
                TransferDataTest() : Service{mSpecifier}
                {
                }
            };

            core::InstanceSpecifier TransferDataTest::mSpecifier{"Instance0"};

            TEST_F(TransferDataTest, ValidTransferConfiguration)
            {
                const TransferDirection cTransferDirection{TransferDirection::kDownload};
                const size_t cMemoryAddress{512};
                const size_t cMemorySize{64};

                EXPECT_TRUE(
                    Service.TrySetTransferConfiguration(
                        cTransferDirection, cMemoryAddress, cMemorySize));

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cTransferDirection, cMemoryAddress, cMemorySize));
            }

            TEST_F(TransferDataTest, InvalidTransferConfiguration)
            {
                const TransferDirection cValidTransferDirection{TransferDirection::kDownload};
                const size_t cValidMemoryAddress{512};
                const size_t cValidMemorySize{64};

                const TransferDirection cInvalidTransferDirection{TransferDirection::kNone};
                const size_t cInvalidMemoryAddress{2048};
                const size_t cInvalidMemorySize{1024};
                const size_t cZeroMemorySize{0};

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cInvalidTransferDirection, cValidMemoryAddress, cValidMemorySize));

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cValidTransferDirection, cInvalidMemoryAddress, cValidMemorySize));

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cValidTransferDirection, cValidMemoryAddress, cZeroMemorySize));

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cValidTransferDirection, cValidMemoryAddress, cInvalidMemorySize));
            }

            TEST_F(TransferDataTest, TransferConfigurationReset)
            {
                const TransferDirection cTransferDirection{TransferDirection::kDownload};
                const size_t cMemoryAddress{512};
                const size_t cMemorySize{64};

                EXPECT_FALSE(Service.TryResetTransferConfiguration());

                Service.TrySetTransferConfiguration(
                    cTransferDirection, cMemoryAddress, cMemorySize);

                EXPECT_TRUE(Service.TryResetTransferConfiguration());
            }

            TEST_F(TransferDataTest, IncorrectMessageLengthScenario)
            {
                const uint8_t cExpectedNrc{0x13};

                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{Service.GetSid()};
                bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }

            TEST_F(TransferDataTest, RequestSequenceErrorScenario)
            {
                const uint8_t cExpectedNrc{0x24};
                const uint8_t cBlockSequenceCounter{1};

                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{Service.GetSid(), cBlockSequenceCounter};
                bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }

            TEST_F(TransferDataTest, WrongBlockSequenceCounterScenario)
            {
                const TransferDirection cTransferDirection{TransferDirection::kDownload};
                const size_t cMemoryAddress{512};
                const size_t cMemorySize{64};
                const uint8_t cExpectedNrc{0x73};
                const uint8_t cSmallerBlockSequenceCounter{0};
                const uint8_t cGreaterBlockSequenceCounter{2};

                Service.TrySetTransferConfiguration(
                    cTransferDirection, cMemoryAddress, cMemorySize);

                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{Service.GetSid(), cSmallerBlockSequenceCounter};
                bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);

                _requestData = {Service.GetSid(), cGreaterBlockSequenceCounter};
                _hasNrc = TryGetNrc(&Service, _requestData, _actualNrc);

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }
        }
    }
}