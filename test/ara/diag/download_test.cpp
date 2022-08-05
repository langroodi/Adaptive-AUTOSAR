#include "./routing/testable_uds_service.h"
#include "../../../src/ara/diag/download.h"

namespace ara
{
    namespace diag
    {
        class DownloadServiceTest : public routing::TestableUdsService
        {
        private:
            static core::InstanceSpecifier mSpecifier;
            static const ReentrancyType cReentrancy{ReentrancyType::kNot};

            const std::string cMaxNumberOfBlockLengthKey{"MaxNumberOfBlockLength"};
            const std::string cMaxNumberOfBlockLengthStr{"64"};

            routing::TransferData mTransferData;

        protected:
            DownloadService Service;

            bool TryBeginTransferData()
            {
                const routing::TransferDirection cTransferDirection{routing::TransferDirection::kDownload};
                const size_t cMemoryAddress{512};
                const size_t cMemorySize{64};

                bool _result(
                    mTransferData.TrySetTransferConfiguration(
                        cTransferDirection, cMemoryAddress, cMemorySize));

                return _result;
            }

        public:
            DownloadServiceTest() : mTransferData(mSpecifier),
                                    Service(mSpecifier, cReentrancy, mTransferData)
            {
                GeneralMetaInfo.SetValue(
                    cMaxNumberOfBlockLengthKey, cMaxNumberOfBlockLengthStr);
            }
        };

        core::InstanceSpecifier DownloadServiceTest::mSpecifier{"Instance0"};
        const ReentrancyType DownloadServiceTest::cReentrancy;

        TEST_F(DownloadServiceTest, IncorrectRequestScenario)
        {
            const uint8_t cExpectedNrc{0x13};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid()};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(DownloadServiceTest, OutOfRangeRequestScenario)
        {
            const uint8_t cExpectedNrc{0x31};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid(), 0xff, 0x22, 0x00};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(DownloadServiceTest, NotAcceptedRequestScenario)
        {
            const uint8_t cExpectedNrc{0x70};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid(), 0x00, 0x22, 0xff, 0xff, 0xff, 0xff};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(DownloadServiceTest, AcceptedRequestScenario)
        {
            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid(), 0x00, 0x22, 0x00, 0x01, 0x00, 0x01};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_FALSE(_hasNrc);
        }
    }
}